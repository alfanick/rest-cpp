#include <rest/rest.h>
#include "resources/list.cpp"
#include "resources/task.cpp"

void routes(REST::Router* r) {
  r->resource<List>("/");
  r->resource<List>("/:list_id");
  r->resources<Task>("/:list_id/task");
}

