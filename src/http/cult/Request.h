#pragma once
#include "http/common.h"
namespace cultlang {
namespace http {
	class Request
		: public craft::types::Object
	{
		CULTLANG_HTTP_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::http::Request);

	public:
		craft::instance<std::string> path;
		craft::instance<std::string> method;
		craft::instance<craft::lisp::library::Map> headers;
		craft::types::instance<> body;
	public:

		CULTLANG_HTTP_EXPORTED Request();
	};
}}