// -*- C++ -*-

#include "escalator.h"
#include "material.h"
#include "model.h"
#include "physics/particle.h"
#include "physics/shape.h"
#include "physics/vector.h"

using namespace mbostock;

Escalator::Escalator(const Vector& min, const Vector& max, const Vector& v)
    : box_(min, max), velocity_(v * ParticleSimulator::timeStep()),
      model_(box_) {
  model_.setTexOrientation((fabsf(v.x) > fabsf(v.z))
      ? ((v.x > 0) ? AxisAlignedBoxModel::POSITIVE_X
                   : AxisAlignedBoxModel::NEGATIVE_X)
      : ((v.z > 0) ? AxisAlignedBoxModel::POSITIVE_Z
                   : AxisAlignedBoxModel::NEGATIVE_Z));
}

Model& Escalator::model() {
  return model_;
}

const Shape& Escalator::shape() const {
  return box_;
}

void Escalator::step(const ParticleSimulator& s) {
  offset_.x = fmodf(offset_.x + velocity_.x, 1.f);
  offset_.z = fmodf(offset_.z + velocity_.z, 1.f);
  model_.setTexOffset(-offset_.x, -offset_.z);
}

Vector Escalator::velocity(const Vector& x) const {
  return velocity_;
}

void Escalator::setMaterial(const Material& m) {
  model_.setMaterial(m);
}

void Escalator::setTopMaterial(const Material& m) {
  model_.setTopMaterial(m);
}

float Escalator::slip() const {
  return model_.material().slip();
}
