#include "http_req.h"

void HttpReq::sendHtmlResponse(const int p_code, const String p_content) {
	CharString content = p_content.utf8();
	const char *data = content.get_data();

	// Send HTTP reply to the client
	httplib_printf(ctx, conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/html; charset=utf-8\r\n"
			"Content-Length: %d\r\n" // Always set Content-Length
			"\r\n"
			"%s",
			content.size() - 1,
			data);

	_response_send = true;
}

void HttpReq::sendTextResponse(const int p_code, const String p_content) {
	CharString content = p_content.utf8();
	const char *data = content.get_data();

	// Send HTTP reply to the client
	httplib_printf(ctx, conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/plain; charset=utf-8\r\n"
			"Content-Length: %d\r\n" // Always set Content-Length
			"\r\n"
			"%s",
			content.size() - 1,
			data);

	_response_send = true;
}

StringName HttpReq::get_path() {
	const struct lh_rqi_t *request_info = httplib_get_request_info(conn);
	return StaticCString::create(request_info->uri);
}

StringName HttpReq::get_query() {
	const struct lh_rqi_t *request_info = httplib_get_request_info(conn);
	return StaticCString::create(request_info->query_string);
}

bool HttpReq::is_response_send() {
	return _response_send;
}

void HttpReq::_bind_methods() {
	ClassDB::bind_method(D_METHOD("sendTextResponse", "code", "content"), &HttpReq::sendTextResponse);
	ClassDB::bind_method(D_METHOD("sendHtmlResponse", "code", "content"), &HttpReq::sendHtmlResponse);
	ClassDB::bind_method(D_METHOD("get_path"), &HttpReq::get_path);
	ClassDB::bind_method(D_METHOD("get_query"), &HttpReq::get_query);
}

void HttpReq::create(lh_ctx_t *p_ctx, lh_con_t *p_conn) {
	ctx = p_ctx;
	conn = p_conn;
	_response_send = false;
}

HttpReq::HttpReq() {
}
