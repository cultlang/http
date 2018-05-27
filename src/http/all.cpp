#include "http/common.h"

#include "all.h"

#include "lisp/library/system/prelude.h"
#include "lisp/semantics/cult/calling.h"

#include "cult/Request.h"
#include "cult/Response.h"

#include "sundown/markdown.h"
#include "sundown/buffer.h"
#include "sundown/html.h"

using namespace craft;
using namespace craft::lisp;
using namespace craft::types;
using namespace cultlang::http;



#define lMM sem->builtin_implementMultiMethod
#define HtP "http"


#include "typedefs.h"

instance<Module> cultlang::http::make_http_bindings(instance<lisp::Namespace> ns, instance<> loader)
{
	auto ret = instance<Module>::make(ns, loader);
	auto sem = instance<CultSemantics>::make(ret);
	ret->builtin_setSemantics(sem);

	lMM(HtP"/request", []() { return t_req::make();});
	lMM(HtP"/request", [](t_str s) { return cpptype<a_req>::typeDesc().getFeature<PParse>()->parse(*s); });
	
	lMM(HtP"/request/path", [](t_req r) { return r->path; });
	lMM(HtP"/request/path", [](t_req r, t_str i) { r->path = i; });

	lMM(HtP"/request/headers", [](t_req r) { return r->headers; });
	lMM(HtP"/request/headers", [](t_req r, instance<library::Map> m) { r->headers = m; });

	lMM(HtP"/request/body", [](t_req r) { return r->body; });
	lMM(HtP"/request/body", [](t_req r, instance<> i) { r->body = i; });

	lMM(HtP"/response", []() { return t_rep::make(); });
	lMM(HtP"/response", [](t_str s) { return cpptype<a_rep>::typeDesc().getFeature<PParse>()->parse(*s); });

	
	lMM(HtP"/response/code", [](t_rep r) { return r->code; });
	lMM(HtP"/response/code", [](t_rep r, t_u16 i) { r->code = i; });

	lMM(HtP"/response/headers", [](t_rep r) { return r->headers; });
	lMM(HtP"/response/headers", [](t_rep r, instance<library::Map> m) { r->headers = m; });

	lMM(HtP"/response/body", [](t_rep r) { return r->body; });
	lMM(HtP"/response/body", [](t_rep r, instance<> i) { r->body = i; });

	lMM(HtP"/markdown/parse", [](t_str i) { 
		struct buf *ib, *ob;
		int ret;
		FILE *in = stdin;

		struct sd_callbacks callbacks;
		struct html_renderopt options;
		struct sd_markdown *markdown;

		sdhtml_renderer(&callbacks, &options, 0);
		markdown = sd_markdown_new(0, 16, &callbacks, &options);
		ob = bufnew(i->size());
		bufgrow(ob, 1024);

		sd_markdown_render(ob, (uint8_t*)i->data(), i->size(), markdown);
		sd_markdown_free(markdown);

		/* writing the result to stdout */

		auto res = instance<std::string>::make((char*)ob->data, ob->size);

		/* cleanup */
		bufrelease(ob);
		return res;
	});
	return ret;
}

BuiltinModuleDescription cultlang::http::BuiltinHttp("cult/http", cultlang::http::make_http_bindings);


#include "types/dll_entry.inc"
