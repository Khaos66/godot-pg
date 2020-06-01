#include "http_server.h"
#include "http_req.h"

Map<lh_ctx_t *, HttpServer *> HttpServer::servers;

bool HttpServer::start(const String p_ports) {
	// List of options. Last element must be NULL.
	const lh_opt_t options[] = { { "listening_ports", p_ports.ascii().get_data() }, NULL };

	struct lh_clb_t callbacks;
	memset(&callbacks, 0, sizeof(callbacks));
	callbacks.begin_request = &HttpServer::_begin_request;

	ctx = httplib_start(&callbacks, NULL, options);

	if (!ctx) {
		WARN_PRINT("Can't start HTTP server");
		return false;
	}

	servers.insert(ctx, this);
	return true;
}

bool HttpServer::stop() {
	if (ctx) {
		httplib_stop(ctx);
		ctx = NULL;
	}
	return true;
}

int HttpServer::_begin_request(lh_ctx_t *p_ctx, lh_con_t *p_conn) {
	const Map<lh_ctx_t *, HttpServer *>::Element *server = servers.find(p_ctx);
	if (!server) {
		ERR_PRINT("Got Request but can't find HttpServer!");
		return -1;
	}
	return server->value()->emit_begin_request(p_ctx, p_conn);
}

int HttpServer::emit_begin_request(lh_ctx_t *p_ctx, lh_con_t *p_conn) {
	Ref<HttpReq> req;
	req.instance();
	req->create(p_ctx, p_conn);

	emit_signal("begin_request", req);

	if (req->is_response_send()) {
		return 1;
	}
	return 0;
}

void HttpServer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("start", "ports"), &HttpServer::start);
	ClassDB::bind_method(D_METHOD("stop"), &HttpServer::stop);

	ADD_SIGNAL(MethodInfo("begin_request", PropertyInfo(Variant::OBJECT, "request")));
}

HttpServer::HttpServer() {
	servers = Map<lh_ctx_t *, HttpServer *>();
}
