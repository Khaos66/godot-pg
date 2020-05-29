#ifndef HTTP_REQ_H
#define HTTP_REQ_H

#include "core/resource.h"
#include "libhttp.h"

class HttpReq : public Reference {
	GDCLASS(HttpReq, Reference);

	struct lh_ctx_t *ctx;
	struct lh_con_t *conn;

	bool _response_send;

protected:
	static void _bind_methods();

public:
	void sendResponse(const int p_code, const String p_type, const String p_content);
	bool is_response_send();
	HttpReq(lh_ctx_t *p_ctx, lh_con_t *p_conn);
	HttpReq();
};

#endif // HTTP_SERVER_H
