#include "_base.h"

class List : public BaseResource {
  /**
   * Send names and ids of todo lists
   */
  void read() {
    auto& object = response->data["folder"];
    object["owner"] = request->authorization.first;
    object["splat"] = request->parameter("0", std::string(""));
  }
  /**
   * Remove todo list
   */
  void destroy() {

  }

  /**
   * Rename todolist
   */
  void update() {

  }

  /**
   * create new todolist
   */
  void create() {

  }
};
