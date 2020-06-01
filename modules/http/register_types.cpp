#include "register_types.h"

#include "core/class_db.h"
#include "http_req.h"
#include "http_server.h"

void register_http_types() {
	ClassDB::register_class<HttpServer>();
	ClassDB::register_class<HttpReq>();
}

void unregister_http_types() {}
