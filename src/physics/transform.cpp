// -*- C++ -*-

#include "transform.h"

using namespace mbostock;

Transform::Transform()
    : enabled_(true) {
}

void Transform::enable(bool enabled) {
  enabled_ = enabled;
}
