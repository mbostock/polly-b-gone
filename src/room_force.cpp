// -*- C++ -*-

#include "room_force.h"
#include "physics/particle.h"
#include "physics/vector.h"

using namespace mbostock;

ConstantRoomForce::ConstantRoomForce(const Vector& min, const Vector& max,
                                     const Vector& f)
    : box_(min, max), force_(f) {
}

const Shape& ConstantRoomForce::shape() const {
  return box_;
}

Vector ConstantRoomForce::force(const Particle& p) {
  return box_.contains(p.position) ? force_ : Vector::ZERO();
}
