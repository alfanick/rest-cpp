#include <rest/rest.h>
#include "resources/list.cpp"
#include "resources/task.cpp"

void routes(REST::Router* r) {
  r->resource<List>("/");
  r->resource<List>("/:list_id");
  r->resources<Task>("/:list_id/task");
  r->match("/foo", [](REST::LambdaService* s) {
    s->ensure_session();

    if (s->session->data["counter"].empty())
      s->session->data["counter"] = 0;

    s->session->data["counter"] = s->session->data["counter"].asLargestUInt() + 1;

    s->response->use_json();
    s->response->data = s->session->data;
  });
}

