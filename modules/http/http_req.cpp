#include "http_req.h"
#include <cstring>
#include <string.h>

void HttpReq::sendFile(const String p_path, const String p_mime_type) {
	// Godot strings have to be copied in order to work... IDK WHY!!
	const char *p_filepath = p_path.utf8().get_data();
	char *filepath = (char *)memalloc(strlen(p_filepath) + 1);
	std::memcpy(filepath, p_filepath, strlen(p_filepath) + 1);
	// Also type string
	const char *p_type = p_mime_type.ascii().get_data();
	char *type = (char *)memalloc(strlen(p_type) + 1);
	std::memcpy(type, p_type, strlen(p_type) + 1);

	const char *headers = { NULL };
	httplib_send_file(ctx, conn, filepath, type, headers);

	_response_send = true;
}

void HttpReq::sendHtml(const int p_code, const String p_content) {
	CharString content = p_content.utf8();
	// Godot strings have to be copied in order to work... IDK WHY!!
	const char *p_data = content.get_data();
	char *data = (char *)memalloc(strlen(p_data) + 1);
	std::memcpy(data, p_data, strlen(p_data) + 1);
	// Same here
	const char *p_codeData = p_content.ascii().get_data();
	char *codeData = (char *)memalloc(strlen(p_codeData) + 1);
	std::memcpy(codeData, p_codeData, strlen(p_codeData) + 1);

	// Send HTTP reply to the client
	httplib_printf(ctx, conn,
			"HTTP/1.1 %s OK\r\n"
			"Content-Type: text/html; charset=utf-8\r\n"
			"Content-Length: %d\r\n" // Always set Content-Length
			"\r\n"
			"%s",
			codeData,
			content.size() - 1,
			data);

	_response_send = true;
}

void HttpReq::sendText(const String p_content) {
	CharString content = p_content.utf8();
	// Godot strings have to be copied in order to work... IDK WHY!!
	const char *p_data = content.get_data();
	char *data = (char *)memalloc(strlen(p_data) + 1);
	std::memcpy(data, p_data, strlen(p_data) + 1);

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

void HttpReq::sendJson(const String p_content) {
	CharString content = p_content.utf8();
	// Godot strings have to be copied in order to work... IDK WHY!!
	const char *p_data = content.get_data();
	char *data = (char *)memalloc(strlen(p_data) + 1);
	std::memcpy(data, p_data, strlen(p_data) + 1);

	// Send HTTP reply to the client
	httplib_printf(ctx, conn,
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: application/json; charset=utf-8\r\n"
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
	ClassDB::bind_method(D_METHOD("sendFile", "path", "mime_type"), &HttpReq::sendFile);
	ClassDB::bind_method(D_METHOD("sendHtml", "code", "content"), &HttpReq::sendHtml);
	ClassDB::bind_method(D_METHOD("sendText", "content"), &HttpReq::sendText);
	ClassDB::bind_method(D_METHOD("sendJson", "content"), &HttpReq::sendJson);
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
