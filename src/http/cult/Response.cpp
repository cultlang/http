#include "http/common.h"
#include "Response.h"

using namespace craft;
using namespace craft::types;
using namespace craft::lisp;
using namespace cultlang::http;

std::string httpCode(int16_t i) {
	switch (i) {
		case 100: return "CONTINUE";
		case 101: return "SWITCHING_PROTOCOLS";
		case 102: return "PROCESSING";
		case 200: return "OK";
		case 201: return "CREATED";
		case 202: return "ACCEPTED";
		case 203: return "NON_AUTHORITATIVE_INFORMATION";
		case 204: return "NO_CONTENT";
		case 205: return "RESET_CONTENT";
		case 206: return "PARTIAL_CONTENT";
		case 207: return "MULTI_STATUS";
		case 208: return "ALREADY_REPORTED";
		case 226: return "IM_USED";
		case 300: return "MULTIPLE_CHOICES";
		case 301: return "MOVED_PERMANENTLY";
		case 302: return "FOUND";
		case 303: return "SEE_OTHER";
		case 304: return "NOT_MODIFIED";
		case 305: return "USE_PROXY";
		case 307: return "TEMPORARY_REDIRECT";
		case 308: return "PERMANENT_REDIRECT";
		case 400: return "BAD_REQUEST";
		case 401: return "UNAUTHORIZED";
		case 402: return "PAYMENT_REQUIRED";
		case 403: return "FORBIDDEN";
		case 404: return "NOT_FOUND";
		case 405: return "METHOD_NOT_ALLOWED";
		case 406: return "NOT_ACCEPTABLE";
		case 407: return "PROXY_AUTHENTICATION_REQUIRED";
		case 408: return "REQUEST_TIMEOUT";
		case 409: return "CONFLICT";
		case 410: return "GONE";
		case 411: return "LENGTH_REQUIRED";
		case 412: return "PRECONDITION_FAILED";
		case 413: return "PAYLOAD_TOO_LARGE";
		case 414: return "URI_TOO_LONG";
		case 415: return "UNSUPPORTED_MEDIA_TYPE";
		case 416: return "RANGE_NOT_SATISFIABLE";
		case 417: return "EXPECTATION_FAILED";
		case 421: return "MISDIRECTED_REQUEST";
		case 422: return "UNPROCESSABLE_ENTITY";
		case 423: return "LOCKED";
		case 424: return "FAILED_DEPENDENCY";
		case 426: return "UPGRADE_REQUIRED";
		case 428: return "PRECONDITION_REQUIRED";
		case 429: return "TOO_MANY_REQUESTS";
		case 431: return "REQUEST_HEADER_FIELDS_TOO_LARGE";
		case 451: return "UNAVAILABLE_FOR_LEGAL_REASONS";
		case 500: return "INTERNAL_SERVER_ERROR";
		case 501: return "NOT_IMPLEMENTED";
		case 502: return "BAD_GATEWAY";
		case 503: return "SERVICE_UNAVAILABLE";
		case 504: return "GATEWAY_TIMEOUT";
		case 505: return "HTTP_VERSION_NOT_SUPPORTED";
		case 506: return "VARIANT_ALSO_NEGOTIATES";
		case 507: return "INSUFFICIENT_STORAGE";
		case 508: return "LOOP_DETECTED";
		case 510: return "NOT_EXTENDED";
		case 511: return "NETWORK_AUTHENTICATION_REQUIRED";
		default: return "";
	}
}

namespace _impl {
	std::regex rep_regex("HTTP/1.1 (\\d+) (.+)");

}

CRAFT_DEFINE(Response)
{
	_.use<PStringer>().singleton<FunctionalStringer>(
		[](::craft::instance<Response> _this) {
		std::ostringstream res;
		res << "HTTP/1.1" << ' ' << *_this->code << ' ' << httpCode(*_this->code) << "\r\n";
		for (auto h : _this->headers->data())
		{
			res << h.first.toString() << ": " << h.second.toString() << "\r\n";
		}
		res << "\r\n";
		res << _this->body.toString();
		return res.str();
	});
	_.use<PParse>().singleton<FunctionalParse>([](std::string s) {
		auto res = instance<Response>::make();
		auto l1 = s.find("\r\n");
		std::smatch base_match;
		std::string line = s.substr(0, l1);
		if (!std::regex_match(line, base_match, _impl::rep_regex))
		{
			throw stdext::exception("Parse Error in Start Line");
		}
		res->code = instance<uint16_t>::make(uint16_t(std::stol(base_match[1])));
		

		auto l2 = s.find("\r\n\r\n");
		std::string headers = s.substr(l1 + 2, l2);
		std::vector<std::string> h;
		stdext::split(headers, "\r\n", std::back_inserter(h));
		for (auto line : h) {
			if (line == "") continue;
			auto ind = line.find(":");
			if (ind == line.npos) continue;
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

Response::Response()
{
	headers = instance<library::Map>::make();
}