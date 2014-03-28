#include "feature.h"

namespace REST {

Feature::Feature() {
  features.push_back(this);
}

void Feature::feature_push() {
}

void Feature::feature_pop() {

}

};
