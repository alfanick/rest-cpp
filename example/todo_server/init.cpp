#include <rest/rest.h>

#include "resources/hello_world_service.cpp"

create_json_service(adder) {
  int a = service->request->parameter("a", 0);
  int b = service->request->parameter("b", 0);

  service->response->data["result"] = a+b;
}

create_service(hole) {
  throw MethodNotAllowed();
}

create_service(secret) {
  service->ensure_authorization("Some secret", [](std::string username, std::string password) -> bool {
    return username == "root" && password == "spam";
  });

  service->response->raw = "You are awesome!";
}

void routes(REST::Router* r) {
  r->route("/lol/foo");
  r->route("/task/:id");
  r->route("/path/:bar/*");
  r->route("/path/:bar/*");
  r->route("/path/:bar/edit");
  r->route("/path/:bar/*");
  r->route("/");
  r->route("/*");
  r->route("/:foo");

  REST::params_map params;

  r->match("/task/17", params);
  //r->match("/foo/bar", params);
  //r->match("/path/12/edit", params);
  r->match("/path/12/69/lol", params);
  std::cout <<"start\n";
  r->match("/", params);

  for (auto p : params) {
    std::cout << "'"<<p.first<<"': '"<<p.second<<"'\n";
  }
  //r->match("/path/lol/edit", params);

  r->resource<HelloWorldService>("przywitanie");

  r->path("lol", hole);
  r->path("sum/:a/:b", adder);
  r->path("secret", secret);

  r->path("fibonacci/:fib", [](REST::Service* service) {
    int num = service->request->parameter("fib", 0);
    int fib1=1, fib2=1, res=0;
    if(num ==0 || num == 1 || num == 2)
      res = 1;
    else {
      num -= 2;
      while(num--) {
        res = fib1 + fib2;
        fib1 = fib2;
        fib2 = res;
      }
    }
    service->response->use_json();
    service->response->data["result"] = res;
  });
}
