#pragma once
#include "http/common.h"
namespace cultlang {
namespace http {
	class Response
		: public craft::types::Object
	{
		CULTLANG_HTTP_EXPORTED CRAFT_OBJECT_DECLARE(cultlang::http::Response);

	public:
		craft::instance<uint16_t> code;
		craft::instance<craft::lisp::library::Map> headers;
		craft::types::instance<> body;
	public:
		CULTLANG_HTTP_EXPORTED Response();
	};
}}