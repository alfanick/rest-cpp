#include <rest/resource.h>

class Task : public REST::Resource {
  void read() {
    throw REST::HTTP::NotImplemented();
  }
};