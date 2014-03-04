#include "_base.h"

class List : public BaseResource {
  void read() {
    throw REST::HTTP::NotImplemented();
  }
};
