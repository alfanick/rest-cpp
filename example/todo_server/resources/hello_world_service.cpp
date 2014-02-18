#include <rest/resource.h>
#include <iostream>

class HelloWorldService : public REST::Resource {
  public:
    void before() {
//      ensure_session();
      response->use_json();
    }

    void create() {
      response->data["foo"] = "bar";
    }

    void read() {
      response->data["message"] = "Hello " + (request->parameters["1"].empty() ? "someone" : request->parameters["1"]) + "!";
      response->data["splat"] = request->parameters["0"];
 //     response->data["name"] = "Name z sesji to " + session->data["name"];
 //     if (session->data["name"].empty())
 //       session->data["name"] = request->parameters["0"];
    }
};

