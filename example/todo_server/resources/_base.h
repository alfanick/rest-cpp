#ifndef _BASE_H
#define _BASE_H

#include <rest/resource.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

class BaseResource : public REST::Resource {
  void before() {
    response->use_json();
    ensure_authorization("Need to authorize", [this](std::string username, std::string password) {
      std::ifstream password_file("./data/" + username + "/.password");
      bool authorized = false;

      if (password_file.is_open()) {
        std::string saved_password;

        password_file >> saved_password;
        password_file.close();

        authorized = (password == saved_password);
      } else {
        mkdir(("./data/" + username).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        std::ofstream new_password_file("./data/" + username + "/.password");

        new_password_file << password;
        new_password_file.close();

        authorized = true;
      }

      if (authorized)
        data_path = "./data/" + username;

      return authorized;
    });
  }

  protected:
    std::string data_path;
};

#endif
