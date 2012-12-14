// -*- C++ -*-

#include <algorithm>
#include <stdlib.h>

#include "vector.h"

using namespace mbostock;

const Vector& Vector::ZERO() {
  static const Vector v;
  return v;
}

const Vector& Vector::X() {
  static const Vector v(1.f, 0.f, 0.f);
  return v;
}

const Vector& Vector::Y() {
  static const Vector v(0.f, 1.f, 0.f);
  return v;
}

const Vector& Vector::Z() {
  static const Vector v(0.f, 0.f, 1.f);
  return v;
}

const Vector& Vector::INF() {
  static const Vector v(INFINITY, INFINITY, INFINITY);
  return v;
}

static float randomf() {
  return random() / (float) RAND_MAX;
}

Vector Vector::min(const Vector& a, const Vector& b) {
  return Vector(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Vector Vector::max(const Vector& a, const Vector& b) {
  return Vector(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

Vector Vector::randomVector(float k) {
  Vector v(randomf() - .5f, randomf() - .5f, randomf() - .5f);
  return ((v.x == 0.f) && (v.y == 0.f) && (v.z == 0.f))
    ? Vector(k, 0.f, 0.f) // very unlikely, but still...
    : v.normalize() * k;
}
