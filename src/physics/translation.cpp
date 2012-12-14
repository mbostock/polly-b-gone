// -*- C++ -*-

#include "particle.h"
#include "translation.h"
#include "vector.h"

using namespace mbostock;

Translation::Translation(const Vector& x0, const Vector& x1,
                         float speed, float start, float dampen)
    : x0_(x0), x1_(x1), u_(start), s_(speed * ParticleSimulator::timeStep()),
      kd_(1.f - dampen), v_((x1 - x0) * s_),
      x_(x0 * (1.f - u_) + x1 * u_), origin_(x_),
      mode_(REVERSE), reversed_(false) {
}

void Translation::reset() {
  origin_ = x_ = x0_ * (1.f - u_) + x1_ * u_;
  v_ = (x1_ - x0_) * s_;
  reversed_ = false;
}

void Translation::step() {
  if (!enabled()) {
    return;
  }
  Vector x = origin_ + v_;
  if (reversed_) {
    if (v_.dot(x0_ - x) < 0.f) {
      reversed_ = false;
      v_ *= -1.f;
    }
  } else {
    if (v_.dot(x1_ - x) < 0.f) {
      switch (mode_) {
        case REVERSE: {
          reversed_ = true;
          v_ *= -1.f;
          break;
        }
        case RESET: {
          x_ = x0_;
          origin_ = x0_;
          break;
        }
        case ONE_WAY: {
          v_ = Vector::ZERO();
          break;
        }
      }
    }
  }
  x_ += v_;
  dv_ = (x_ - origin_) * kd_;
  origin_ += dv_;
}

void Translation::setMode(Mode m) {
  mode_ = m;
}

const Vector& Translation::velocity() const {
  return enabled() ? dv_ : Vector::ZERO();
}

Vector Translation::point(const Vector& x) const {
  return x + origin_;
}

Vector Translation::pointInverse(const Vector& x) const {
  return x - origin_;
}

TranslatingShape::TranslatingShape(const Shape& s, const Translation& t)
    : shape_(s), translation_(t) {
}

bool TranslatingShape::intersects(const Sphere& s) const {
  Sphere ts(translation_.pointInverse(s.x()), s.radius());
  return shape_.intersects(ts);
}

Projection TranslatingShape::project(const Vector& x) const {
  Projection p = shape_.project(translation_.pointInverse(x));
  p.x = translation_.point(p.x);
  return p;
}
