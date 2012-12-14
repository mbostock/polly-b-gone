// -*- C++ -*-

#include <math.h>

#include "particle.h"
#include "rotation.h"

using namespace mbostock;

Rotation::Rotation(const Vector& origin, const Vector& axis,
                   float speed, float angle)
    : origin_(origin), axis_(axis), startAngle_(angle),
      speed_(speed * ParticleSimulator::timeStep()), angle_(angle) {
  update();
}

void Rotation::reset() {
  angle_ = startAngle_;
}

void Rotation::step() {
  if (!enabled()) {
    return;
  }
  angle_ += speed_;
  angle_ = fmodf(angle_, 360.f);
  update();
}

void Rotation::update() {
  float r = angle_ * (2.f * M_PI / 360.f);
  float c = cosf(r);
  float s = sinf(r);

  /* This look familiar to anyone? /wink */
  matrix_[0] = axis_.x * axis_.x * (1.f - c) + c;
  matrix_[1] = axis_.x * axis_.y * (1.f - c) - axis_.z * s;
  matrix_[2] = axis_.x * axis_.z * (1.f - c) + axis_.y * s;
  matrix_[3] = axis_.y * axis_.x * (1.f - c) + axis_.z * s;
  matrix_[4] = axis_.y * axis_.y * (1.f - c) + c;
  matrix_[5] = axis_.y * axis_.z * (1.f - c) - axis_.x * s;
  matrix_[6] = axis_.x * axis_.z * (1.f - c) - axis_.y * s;
  matrix_[7] = axis_.y * axis_.z * (1.f - c) + axis_.x * s;
  matrix_[8] = axis_.z * axis_.z * (1.f - c) + c;
}

Vector Rotation::point(const Vector& x) const {
  return origin_ + vector(x - origin_);
}

Vector Rotation::pointInverse(const Vector& x) const {
  return origin_ + vectorInverse(x - origin_);
}

Vector Rotation::vector(const Vector& x) const {
  return Vector(
      matrix_[0] * x.x + matrix_[1] * x.y + matrix_[2] * x.z,
      matrix_[3] * x.x + matrix_[4] * x.y + matrix_[5] * x.z,
      matrix_[6] * x.x + matrix_[7] * x.y + matrix_[8] * x.z);
}

Vector Rotation::vectorInverse(const Vector& x) const {
  return Vector(
      matrix_[0] * x.x + matrix_[3] * x.y + matrix_[6] * x.z,
      matrix_[1] * x.x + matrix_[4] * x.y + matrix_[7] * x.z,
      matrix_[2] * x.x + matrix_[5] * x.y + matrix_[8] * x.z);
}

Vector Rotation::velocity(const Vector& x) const {
  return enabled()
      ? (origin_ - x).cross(axis_) * speed_ * (2.f * M_PI / 360.f)
      : Vector::ZERO();
}

RotatingShape::RotatingShape(const Shape& s, const Rotation& r)
    : shape_(s), rotation_(r) {
}

bool RotatingShape::intersects(const Sphere& s) const {
  Sphere rs(rotation_.pointInverse(s.x()), s.radius());
  return shape_.intersects(rs);
}

Projection RotatingShape::project(const Vector& x) const {
  Projection p = shape_.project(rotation_.pointInverse(x));
  p.x = rotation_.point(p.x);
  p.normal = rotation_.vector(p.normal);
  return p;
}
