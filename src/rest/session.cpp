#include "session.h"

namespace REST {

  sessions_map* Session::sessions = new sessions_map;

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

  sessions_map* Session::Sessions() {
    if(sessions_added > SESSION_CHECK) {
      sessions_added = 0;
      killSessions();
      std::cout << "cleaning sessions\n";
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
      std::cout << "nowa sesja dla '"<< id <<"'\n";
      return s;
    }

    std::cout << "mam sesje dla '"<<id<<"'\n";
    iter->second->modified_at = time(0);
    sessions_lock.unlock();
    return iter->second;
  }

  std::string Session::generateId() {
    std::string result = "";
    for(int i=0; i<64; i++) {
      result += char((rand() % 26) + 97);
    }
    return result;
  }

  void Session::killSessions() {
    for(sessions_map::iterator iter = sessions->begin(); iter != sessions->end();) {
      if(time(0) - iter->second->modified_at > LIFETIME)
        sessions->erase(iter);
      else
        iter++;
    }
  }


}
