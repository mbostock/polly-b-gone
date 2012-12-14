// -*- C++ -*-

#include <math.h>

#include "physics/particle.h"
#include "rotating.h"

using namespace mbostock;

RotatingModel::RotatingModel(Model& m, const Rotation& r)
    : model_(m), rotation_(r) {
}

void RotatingModel::initialize() {
  model_.initialize();
}

void RotatingModel::display() {
  glPushMatrix();
  glTranslatev(rotation_.origin());
  glRotatev(rotation_.angle(), rotation_.axis());
  glTranslatev(-rotation_.origin());
  model_.display();
  glPopMatrix();
}

RotatingRoomObject::RotatingRoomObject(RoomObject* o, const Rotation& r)
    : TransformingRoomObject(o), rotation_(r),
      shape_(o->shape(), r), model_(o->model(), r) {
}

Model& RotatingRoomObject::model() {
  return model_;
}

const Shape& RotatingRoomObject::shape() const {
  return shape_;
}

Vector RotatingRoomObject::velocity(const Vector& x) const {
  Vector v = rotation_.velocity(x);
  if (object_->dynamic()) {
    v += rotation_.vector(object_->velocity(rotation_.pointInverse(x)));
  }
  return v;
}
