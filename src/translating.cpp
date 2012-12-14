// -*- C++ -*-

#include "physics/particle.h"
#include "physics/vector.h"
#include "translating.h"

using namespace mbostock;

TranslatingModel::TranslatingModel(Model& m, const Translation& t)
    : model_(m), translation_(t) {
}

void TranslatingModel::initialize() {
  model_.initialize();
}

void TranslatingModel::display() {
  glPushMatrix();
  glTranslatev(translation_.origin());
  model_.display();
  glPopMatrix();
}

TranslatingRoomObject::TranslatingRoomObject(RoomObject* o, const Translation& t)
    : TransformingRoomObject(o), translation_(t),
      shape_(o->shape(), t), model_(o->model(), t) {
}

Model& TranslatingRoomObject::model() {
  return model_;
}

const Shape& TranslatingRoomObject::shape() const {
  return shape_;
}

Vector TranslatingRoomObject::velocity(const Vector& x) const {
  Vector v = translation_.velocity();
  if (object_->dynamic()) {
    v += object_->velocity(translation_.pointInverse(x));
  }
  return v;
}
