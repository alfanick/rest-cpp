#include "_base.h"

class Task : public BaseResource, public REST::Features::Foo {
  void read() {
    //throw REST::HTTP::NotImplemented();
    response->raw = "foboar";
  }
};
