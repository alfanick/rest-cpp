#include <rest/rest.h>
#include "resources/list.cpp"
#include "resources/task.cpp"

void routes(REST::Router* r) {
  r->resource<List>("/");

  std::atomic_int a(0);
  r->match("/foo", [](REST::LambdaService* s) {
    s->response->raw = "foo";
    // s->response->data["counter"] = a++;
  });
  r->resource<List>("/:list_id");
  r->resources<Task>("/:list_id/task");
  r->match("/:type/*album/:name.jpg", [](REST::LambdaService* s) {
    s->response->data = s->request->parameters;
  });
}

