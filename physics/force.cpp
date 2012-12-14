// -*- C++ -*-

#include <stdio.h>
#include <stdlib.h>

#include "force.h"
#include "particle.h"
#include "vector.h"

using namespace mbostock;

static const float epsilon = 1E-20;

void UnaryForce::apply(Particle& p) {
  p.force += force(p);
}

void BinaryForce::apply(Particle& a, Particle& b) {
  Vector f = force(a, b);
  a.force += f;
  b.force -= f;
}

RandomForce::RandomForce(float k)
    : k_(k) {
}

Vector RandomForce::force(Particle& p) {
  return Vector::randomVector(k_);
}

GravitationalForce::GravitationalForce(float g)
    : g_(g) {
}

Vector GravitationalForce::force(const Particle& p) {
  return Vector(0, -g_ / p.inverseMass, 0);
}

LinearDragForce::LinearDragForce(float kd)
    : kd_(kd) {
}

Vector LinearDragForce::force(const Particle& p) {
  return p.velocity() * -kd_;
}

QuadraticDragForce::QuadraticDragForce(float kd)
    : kd_(kd) {
}

Vector QuadraticDragForce::force(const Particle& p) {
  const Vector& v = p.velocity();
  return v * v.length() * -kd_;
}

HookeForce::HookeForce(float r, float ks)
    : r_(r), ks_(ks) {
}

Vector HookeForce::force(const Particle& a, const Particle& b) {
  Vector l = a.position - b.position;
  float ll = l.length();
  if (ll == 0.f) {
    l = Vector::randomVector(epsilon);
    ll = epsilon;
  }
  return l * -ks_ * (ll - r_) / ll;
}

DampenedHookeForce::DampenedHookeForce(float r, float ks, float kd)
    : r_(r), ks_(ks), kd_(kd) {
}

Vector DampenedHookeForce::force(const Particle& a, const Particle& b) {
  Vector l = a.position - b.position;
  Vector dl = a.velocity() - b.velocity();
  float ll = l.length();
  if (ll == 0.f) {
    l = Vector::randomVector(epsilon);
    ll = epsilon;
  }
  return l * -(ks_ * (ll - r_) + kd_ * dl.dot(l) / ll) / ll;
}

CoulombForce::CoulombForce(float e)
    : e_(e) {
}

Vector CoulombForce::force(const Particle& a, const Particle& b) {
  Vector l = a.position - b.position;
  float ll = l.length();
  return -l * e_ / (ll * ll * ll);
}
