// -*- C++ -*-

#include "transforming.h"

using namespace mbostock;

TransformingRoomObject::TransformingRoomObject(RoomObject* o)
    : object_(o) {
}

TransformingRoomObject::~TransformingRoomObject() {
  delete object_;
}

void TransformingRoomObject::step(const ParticleSimulator& s) {
  object_->step(s);
}

float TransformingRoomObject::slip() const {
  return object_->slip();
}

void TransformingRoomObject::resetForces() {
  object_->resetForces();
}

void TransformingRoomObject::applyForce(UnaryForce& force) {
  object_->applyForce(force); // TODO transform the force
}

void TransformingRoomObject::applyWeight(float w, const Vector& x) {
  object_->applyWeight(w, x); // TODO transform the point
}

void TransformingRoomObject::constrainInternal() {
  object_->constrainInternal();
}

void TransformingRoomObject::reset() {
  object_->reset();
}
