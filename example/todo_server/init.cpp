#include <rest/rest.h>
#include "resources/list.cpp"
#include "resources/task.cpp"

void routes(REST::Router* r) {
  r->resources<List>("/");
  r->resources<Task>("/:list_id/task");
}

