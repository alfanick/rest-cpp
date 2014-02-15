#include <rest/rest.h>
#include <fstream>
#include <cstdlib>

#include "resources/hello_world_service.cpp"

create_json_service(adder) {
  int a = service->request->parameter("a", 0);
  int b = service->request->parameter("b", 0);

  service->response->data["result"] = a+b;

  std::cout << "ok\n";
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

create_service(public_folder) {
  std::string file_path = "./public/" + service->request->parameter("0", std::string(""));

  if (file_path.find("..") != std::string::npos)
    throw Forbidden();

  std::ifstream file(file_path);

  if (!file)
    throw NotFound();

  file.seekg(0, std::ios::end);
  std::streampos length = file.tellg();
  file.seekg(0,std::ios::beg);

  service->response->raw.resize(length);
  file.read(&(service->response->raw[0]), length);
  file.close();

  FILE* mt = popen(("file --mime-type -b " + file_path).c_str(), "r");
  if (mt == NULL) {
    service->response->headers["Content-Type"] = "application/octet-stream";
  } else {
    char mime[128];
    fgets(mime, 127, mt);
    pclose(mt);
    service->response->headers["Content-Type"] = mime;
  }
}

void routes(REST::Router* r) {
  r->resource<HelloWorldService>("/przywitanie");
  r->path("/*", public_folder);

  r->path("/", hole);
  r->path("/lol", hole);
  r->path("/sum/:a/:b", adder);
  r->path("/secret", secret);

  r->path("/fibonacci/:fib", [](REST::Service* service) {
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
