// -*- C++ -*-

#include "ball.h"
#include "material.h"
#include "model.h"
#include "physics/shape.h"
#include "physics/vector.h"
#include "room.h"

using namespace mbostock;

Ball::Ball(const Vector& x, float radius)
    : sphere_(x, radius), model_(sphere_) {
}

Model& Ball::model() {
  return model_;
}

const Shape& Ball::shape() const {
  return sphere_;
}

float Ball::slip() const {
  return model_.material().slip();
}
