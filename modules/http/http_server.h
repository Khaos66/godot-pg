#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "libhttp.h"
#include "scene/main/node.h"

class HttpServer : public Node {
	GDCLASS(HttpServer, Node);

	struct lh_ctx_t *ctx;

	static Map<lh_ctx_t *, HttpServer *> servers;

	int emit_begin_request(lh_ctx_t *p_ctx, lh_con_t *p_conn);

protected:
	static void _bind_methods();
	static int _begin_request(lh_ctx_t *p_ctx, lh_con_t *p_conn);

public:
	bool start(const String p_ports);
	bool stop();

	HttpServer();
};

#endif // HTTP_SERVER_H
