#include "_base.h"
#include <dirent.h>


class List : public BaseResource {
  /**
   * Send names and ids of todo lists
   */
  void read() {
    auto& object = response->data["folder"];
    object["owner"] = authorization.first;

    auto dir = opendir(data_path.c_str());

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
      if (entry->d_name[0] != '.')
        object["lists"].push_back(entry->d_name);

    closedir(dir);
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
