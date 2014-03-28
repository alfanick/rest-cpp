#include "feature.h"

namespace REST {

Feature::Feature() {
  features.push_back(this);
  std::cout << "dopisuje sie\n";
}

void Feature::feature_push() {
  std::cout << "genaral push\n";
}

void Feature::feature_pop() {

}

};
