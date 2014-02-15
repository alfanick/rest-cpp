#include <rest/service.h>
#include <iostream>

class HelloWorldService : public REST::Service {
  public:
    HelloWorldService() {
    }

    void before() {
//      ensure_session();
      response->use_json();
    }

    void create() {
      response->data["foo"] = "bar";
    }

    void read() {
      response->data["message"] = "Hello " + (request->parameters["0"].empty() ? "someone" : request->parameters["0"]) + "!";
 //     response->data["name"] = "Name z sesji to " + session->data["name"];
 //     if (session->data["name"].empty())
 //       session->data["name"] = request->parameters["0"];
    }
};

