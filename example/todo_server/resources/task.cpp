#include "_base.h"

class Task : public BaseResource {
  void read() {
    //throw REST::HTTP::NotImplemented();
    response->raw = "foboar";
  }
};
