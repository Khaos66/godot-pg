#include "http_req.h"

void HttpReq::sendResponse(const int p_code, const String p_type, const String p_content) {
	CharString content = p_content.utf8();
	const char *data = content.get_data();

	// Send HTTP reply to the client
	httplib_printf(ctx, conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: %s; charset=utf-8\r\n"
			"Content-Length: %d\r\n" // Always set Content-Length
			"\r\n"
			"%s",
			p_type.ascii().get_data(),
			content.size(),
			data);

	_response_send = true;
}

bool HttpReq::is_response_send() {
	return _response_send;
}

void HttpReq::_bind_methods() {
	ClassDB::bind_method(D_METHOD("sendResponse", "code", "type", "content"), &HttpReq::sendResponse);
}

HttpReq::HttpReq(lh_ctx_t *p_ctx, lh_con_t *p_conn) {
	ctx = p_ctx;
	conn = p_conn;
	_response_send = false;
}

HttpReq::HttpReq() {
}
