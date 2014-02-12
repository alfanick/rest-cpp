#ifndef REST_CPP_SESSION_H
#define REST_CPP_SESSION_H

#include <map>
#include <iostream>
#include <ctime>
#include "request.h"

namespace REST {

class Session;
  
typedef std::map<std::string, std::shared_ptr<Session> > sessions_map;

/**
 * Session hold data between requests.
 */
class Session {
  public:
    Session(std::shared_ptr<Request> request);
    ~Session();
    static sessions_map* Sessions();
    std::shared_ptr<Session> getSession(std::string);
    void modify();
  private:
    std::string generateId();
    static void killSessions();

    static const int LIFETIME;
    static int last_cleaning;
    std::map<std::string, std::string> *data;
    std::string id;
    int created_at;
    int modified_at;
    static sessions_map* sessions;
};

}

#endif
