#pragma once
#include "http/common.h"

namespace cultlang {
namespace http 
{
	extern craft::lisp::BuiltinModuleDescription BuiltinHttp;

	CULTLANG_HTTP_EXPORTED craft::instance<craft::lisp::Module> make_http_bindings(craft::instance<craft::lisp::Namespace> ns, craft::instance<> loader);
}}

#include "cult/Request.h"
#include "cult/Response.h"
