#include <rest/rest.h>
#include "resources/list.cpp"
#include "resources/task.cpp"

void routes(REST::Router* r) {
  r->resource<List>("/");

  std::atomic_int a(0);
  // int a = 0;
  r->match("/foo", [&a](REST::LambdaService* s) {
    s->response->use_json();
    // a++;
    s->response->data["counter"] = a++;
    // s->response->raw = std::to_string(a++);
  });
  r->resource<List>("/:list_id");
  r->resources<Task>("/:list_id/task");
  r->resource<Task>("/:type/*/:name.jpg");
}

