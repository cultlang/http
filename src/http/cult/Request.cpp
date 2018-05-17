#include "http/common.h"
#include "Request.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace cultlang::http;

namespace _impl {
	std::regex base_regex("(CONNECT|DELETE|GET|HEAD|OPTIONS|PATCH|POST|PUT) ([^\\s]+) (HTTP/1.1)");
}

CRAFT_DEFINE(Request)
{
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<Request> _this) {
		std::ostringstream res;
		res << *_this->method << ' ' << _this->path << ' ' << "HTTP/1.1" << "\r\n";
		for (auto h : _this->headers->data())
		{
			res << h.first.toString() << ": " << h.second.toString() << "\r\n";
		}
		res << "\r\n";
		res << _this->body.toString();
		return res.str(); 
	});
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) { 
		auto res = instance<Request>::make();
		auto l1 = s.find("\r\n");
		std::smatch base_match;
		std::string line = s.substr(0, l1);
		if (!std::regex_match(line, base_match, _impl::base_regex))
		{
			throw stdext::exception("Parse Error in Start Line");
		}
		res->method = instance<std::string>::make(base_match[1]);
		res->path = instance<std::string>::make(base_match[2]);

		auto l2 = s.find("\r\n\r\n");
		std::string headers = s.substr(l1 + 2, l2);
		std::vector<std::string> h;
		stdext::split(headers, "\r\n", std::back_inserter(h));
		for (auto line : h) {
			if (line == "") continue;
			auto ind = line.find(":");
			auto k = instance<std::string>::make(line.substr(0, ind));
			auto v = instance<std::string>::make(line.substr(ind + 1));
			if (v->at(0) == ' ') v->erase(0, 1);
			res->headers->insert(k, v);
		}
		res->body = instance<std::string>::make(s.substr(l2 + 4));

		return res;
	});
	_.defaults();
}

Request::Request()
{
	headers = instance<library::Map>::make();
}