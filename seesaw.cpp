// -*- C++ -*-

#include <OpenGL/gl.h>

#include "material.h"
#include "physics/constraint.h"
#include "physics/force.h"
#include "physics/particle.h"
#include "physics/shape.h"
#include "physics/vector.h"
#include "seesaw.h"

using namespace mbostock;

static const float centerOffset = .1f;

Seesaw::Seesaw(const Vector& min, const Vector& max, float mass)
    : origin_((min + max) / 2.f), size_(max - min), drag_(1.f),
      model_(box_) {
  left_.inverseMass = 1.f / (mass * .1f);
  right_.inverseMass = 1.f / (mass * .1f);
  center_.inverseMass = 1.f / (mass * .8f);
  reset();
}

const Shape& Seesaw::shape() const {
  return box_;
}

Model& Seesaw::model() {
  return model_;
}

void Seesaw::resetForces() {
  left_.force = Vector::ZERO();
  right_.force = Vector::ZERO();
  center_.force = Vector::ZERO();
  drag_.apply(left_);
  drag_.apply(right_);
  drag_.apply(center_);
}

void Seesaw::applyForce(UnaryForce& force) {
  force.apply(left_);
  force.apply(right_);
  force.apply(center_);
}

void Seesaw::applyWeight(float w, const Vector& x) {
  if (x.x < origin_.x) {
    left_.force.y -= w * (2 * (origin_.x - x.x) / size_.x);
  } else {
    right_.force.y += w * (2 * (origin_.x - x.x) / size_.x);
  }
}

void Seesaw::step(const ParticleSimulator& s) {
  s.step(left_);
  s.step(right_);
  s.step(center_);
  updateBox();
}

void Seesaw::updateBox() {
  Vector x = (right_.position - left_.position) / size_.x;
  Vector y = -x.cross(Vector::Z()) * size_.y / 2.f;
  Vector z = Vector::Z() * size_.z / 2.f;
  box_ = Box(
      left_.position - z - y,
      right_.position - z - y,
      right_.position + z - y,
      left_.position + z - y,
      right_.position - z + y,
      left_.position - z + y,
      left_.position + z + y,
      right_.position + z + y);
}

void Seesaw::constrainInternal() {
  Vector v = origin_ - (left_.position + right_.position) / 2.f;
  right_.position += v;
  left_.position += v;

  Constraints::distance(right_, origin_, size_.x / 2.f);
  Constraints::distance(left_, origin_, size_.x / 2.f);
  Constraints::distance(right_, left_, size_.x);
  Constraints::distance(center_, origin_, centerOffset);

  float d = sqrtf(size_.x * size_.x / 4.f + centerOffset * centerOffset);
  Constraints::distance(left_, center_, d);
  Constraints::distance(right_, center_, d);
}

void Seesaw::reset() {
  left_.position = origin_;
  left_.position.x -= size_.x / 2.f;
  right_.position = origin_;
  right_.position.x += size_.x / 2.f;
  center_.position = origin_;
  center_.position.y -= centerOffset;
  left_.previousPosition = left_.position;
  right_.previousPosition = right_.position;
  center_.previousPosition = center_.position;
  updateBox();
}

void Seesaw::setMaterial(const Material& m) {
  model_.setMaterial(m);
}

void Seesaw::setTopMaterial(const Material& m) {
  model_.setTopMaterial(m);
}

float Seesaw::slip() const {
  return model_.material().slip();
}
