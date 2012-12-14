// -*- C++ -*-

#include "material.h"
#include "model.h"
#include "physics/shape.h"
#include "physics/vector.h"
#include "ramp.h"
#include "room.h"

using namespace mbostock;

Ramp::Ramp(const Vector& x0, const Vector& x1,
           const Vector& x2, const Vector& x3)
    : wedge_(x0, x1, x2, x3), model_(wedge_) {
}

Model& Ramp::model() {
  return model_;
}

const Shape& Ramp::shape() const {
  return wedge_;
}

void Ramp::setMaterial(const Material& m) {
  model_.setMaterial(m);
}

void Ramp::setTopMaterial(const Material& m) {
  model_.setTopMaterial(m);
}

float Ramp::slip() const {
  return model_.material().slip();
}
