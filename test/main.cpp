#include "http/common.h"
#include "http/all.h"

using namespace craft;
using namespace craft::types;
using namespace cultlang::http;

std::string req1 = "GET /?foo=foo HTTP/1.1\r\nHost: google.com\r\nUser-Agent: curl / 7.55.1\r\nAccept: */*\r\n\r\n";


std::string rep1 = "HTTP/1.1 301 Moved Permanently\r\n"
"Location: http://www.google.com/\r\n"
"Content-Type: text/html; charset=UTF-8\r\n"
"Date: Thu, 17 May 2018 22:15:02 GMT\r\n"
"Expires: Sat, 16 Jun 2018 22:15:02 GMT\r\n"
"Cache-Control: public, max-age=2592000\r\n"
"Server: gws\r\n"
"Content-Length: 219\r\n"
"X-XSS-Protection: 1; mode=block\r\n"
"X-Frame-Options: SAMEORIGIN\r\n"
"\r\n"
"<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">"
"<TITLE>301 Moved</TITLE></HEAD><BODY>"
"<H1>301 Moved</H1>"
"The document has moved"
"<A HREF=\"http://www.google.com/\">here</A>."
"</BODY></HTML>";

int main(int argc, char** argv) {
	types::boot();

	auto treq1 = instance<Request>::make();
	treq1 = treq1.getFeature<PParse>()->parse(req1);

	auto treq1_r1 = treq1.getFeature<PStringer>()->toString(treq1);
	

	auto trep1 = instance<Response>::make();
	trep1 = trep1.getFeature<PParse>()->parse(rep1);

	auto trep1_r1 = trep1.getFeature<PStringer>()->toString(trep1);
}