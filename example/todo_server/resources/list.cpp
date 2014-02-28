#include <rest/resource.h>

class List : public REST::Resource {
  void read() {
    throw REST::HTTP::NotImplemented();
  }
};