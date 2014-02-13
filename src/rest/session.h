#ifndef REST_CPP_SESSION_H
#define REST_CPP_SESSION_H

#include <mutex>
#include <map>
#include <iostream>
#include <ctime>

namespace REST {

class Session;

class Worker;
typedef std::map<std::string, std::shared_ptr<Session> > sessions_map;

/**
 * Session hold data between requests.
 */
class Session {
  friend class Worker;
  public:
    Session(std::string i);
    ~Session();
    static sessions_map* Sessions();
    static std::shared_ptr<Session> getSession(std::string);

    std::map<std::string, std::string> data;
    static std::string generateId();
    static void killSessions();

  private:
    static const int SESSION_CHECK;
    static std::mutex sessions_lock;
    static const int LIFETIME;
    static int last_cleaning;
    static size_t sessions_added;
    std::string id;
    int created_at;
    int modified_at;
    static sessions_map* sessions;
};

}

#endif
