#ifndef REST_CPP_SESSION_H
#define REST_CPP_SESSION_H

#include <memory>
#include <mutex>
#include <map>
#include <iostream>
#include <ctime>

#include "../feature.h"
#include "../json/json.h"

namespace REST {

class Session;

namespace Features {

class Session : public Feature {
  public:
    virtual std::string feature_name() const { return "session"; }

    // Json::Value &jsession;
    std::shared_ptr<REST::Session> session = nullptr;
    void ensure_session();

    void feature_push();
    void feature_pop();
};

}

/**
 * Session hold data between requests.
 */
class Session {
  friend class Features::Session;
  public:
    Session(std::string i);
    ~Session();
    static std::map< std::string, std::shared_ptr<Session> >* Sessions();
    static std::shared_ptr<Session> getSession(std::string);

    Json::Value data;
    static void killSessions();

  private:
    std::string id;
    static const int SESSION_CHECK;
    static std::mutex sessions_lock;
    static const int LIFETIME;
    static int last_cleaning;
    static size_t sessions_added;
    int created_at;
    int modified_at;
    static std::map< std::string, std::shared_ptr<Session> >* sessions;
};

}

#endif
