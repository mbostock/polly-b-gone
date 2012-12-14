// -*- C++ -*-

#include "material.h"
#include "model.h"
#include "physics/particle.h"
#include "physics/shape.h"
#include "physics/vector.h"
#include "room.h"
#include "tube.h"

using namespace mbostock;

Tube::Tube(const Vector& x0, const Vector& x1, const Vector& y, float radius)
    : cylinder_(x0, x1, radius), y_(y), model_(cylinder_, y_) {
}

Model& Tube::model() {
  return model_;
}

const Shape& Tube::shape() const {
  return cylinder_;
}

void Tube::setMaterial(const Material& m) {
  model_.setMaterial(m);
}

void Tube::setCapMaterial(const Material& m) {
  model_.setCapMaterial(m);
}

float Tube::slip() const {
  return model_.material().slip();
}
