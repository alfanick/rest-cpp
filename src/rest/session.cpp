#include "session.h"

namespace REST {

  std::map<std::string, std::shared_ptr<Session> >* Session::sessions = new std::map<std::string, std::shared_ptr<Session> >;

  // 24 hours
  const int Session::LIFETIME = 86400;
  const int Session::SESSION_CHECK = 1000;
  int Session::last_cleaning = time(0);
  std::mutex Session::sessions_lock;
  size_t Session::sessions_added = 0;

  Session::Session(std::string i) : id(i) {
    created_at = modified_at = time(0);
  }

  Session::~Session() {
  }

  std::map<std::string, std::shared_ptr<Session> >* Session::Sessions() {
    if(sessions_added > SESSION_CHECK) {
      sessions_added = 0;
      killSessions();
      std::cout << "Cleaning sessions\n";
    }
    return sessions;
  }

  std::shared_ptr<Session> Session::getSession(std::string id) {
    sessions_lock.lock();
    auto iter = Sessions()->find(id);

    if(iter == Sessions()->end()) {
      std::shared_ptr<Session> s = std::make_shared<Session>(id);
      sessions_added++;
      Sessions()->insert(std::make_pair(id, s));
      sessions_lock.unlock();
      return s;
    }

    iter->second->modified_at = time(0);
    sessions_lock.unlock();
    return iter->second;
  }

  void Session::killSessions() {
    for(auto iter = sessions->begin(); iter != sessions->end();) {
      if(time(0) - iter->second->modified_at > LIFETIME)
        sessions->erase(iter);
      else
        iter++;
    }
  }


}
