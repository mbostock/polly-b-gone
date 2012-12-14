// -*- C++ -*-

#include <stdio.h>

#include "constraint.h"
#include "particle.h"
#include "shape.h"

using namespace mbostock;

static Projection p;

bool Constraints::minDistance(Particle& a, Vector p, float d) {
  Vector v = p - a.position;
  float l = v.length();
  if (l < d) {
    a.position += v * ((l - d) / l);
    return true;
  }
  return false;
}

bool Constraints::minDistance(Particle& a, Particle& b, float d) {
  Vector v = b.position - a.position;
  float l = v.length();
  if (l < d) {
    v *= (l - d) / ((a.inverseMass + b.inverseMass) * l);
    a.position += v * a.inverseMass;
    b.position -= v * b.inverseMass;
    return true;
  }
  return false;
}

bool Constraints::maxDistance(Particle& a, Vector p, float d) {
  Vector v = p - a.position;
  float l = v.length();
  if (l > d) {
    a.position += v * ((l - d) / l);
    return true;
  }
  return false;
}

bool Constraints::maxDistance(Particle& a, Particle& b, float d) {
  Vector v = b.position - a.position;
  float l = v.length();
  if (l > d) {
    v *= (l - d) / ((a.inverseMass + b.inverseMass) * l);
    a.position += v * a.inverseMass;
    b.position -= v * b.inverseMass;
    return true;
  }
  return false;
}

bool Constraints::distance(Particle& a, Vector p, float d) {
  Vector v = p - a.position;
  float l = v.length();
  a.position += v * ((l - d) / l);
  return l > 0.f;
}

bool Constraints::distance(Particle& a, Particle& b, float d) {
  Vector v = b.position - a.position;
  float l = v.length();
  v *= (l - d) / ((a.inverseMass + b.inverseMass) * l);
  a.position += v * a.inverseMass;
  b.position -= v * b.inverseMass;
  return l > 0.f;
}

bool Constraints::plane(Particle& a, Vector p, Vector n) {
  float d = n.dot(a.position - p);
  if (d < 0) {
    a.position -= n * d;
    return true;
  }
  return false;
}

bool Constraints::plane(Particle& a, Vector p, Vector n, float kr) {
  float d = n.dot(a.position - p);
  if (d < 0) {
    Vector v = a.position - a.previousPosition;
    Vector vNormal = n * n.dot(v);
    Vector vTangent = v - vNormal;
    a.position -= n * d;
    a.previousPosition = a.position - vTangent + vNormal * kr;
    return true;
  }
  return false;
}

/* XXX What if p.length is 0? Use the normal? */

bool Constraints::inside(Particle& a, const Shape& s, float r) {
  p = s.project(a.position);
  if (p.length > -r) {
    a.position = p.x + (p.x - a.position) * (r / p.length);
    return true;
  }
  return false;
}

bool Constraints::inside(Particle& a, const Shape& s, float r, float kr) {
  p = s.project(a.position);
  if (p.length > -r) {
    Vector v = a.position - a.previousPosition;
    Vector vNormal = p.normal * p.normal.dot(v);
    Vector vTangent = v - vNormal;
    a.position = p.x + (p.x - a.position) * (r / p.length);
    a.previousPosition = a.position - vTangent + vNormal * kr;
    return true;
  }
  return false;
}

bool Constraints::outside(Particle& a, const Shape& s, float r) {
  p = s.project(a.position);
  if (p.length < r) {
    a.position = p.x - (p.x - a.position) * (r / p.length);
    return true;
  }
  return false;
}

bool Constraints::outside(Particle& a, const Shape& s, float r, float kr) {
  p = s.project(a.position);
  if (p.length < r) {
    Vector v = a.position - a.previousPosition;
    Vector vNormal = p.normal * p.normal.dot(v);
    Vector vTangent = v - vNormal;
    a.position = p.x - (p.x - a.position) * (r / p.length);
    a.previousPosition = a.position - vTangent + vNormal * kr;
    return true;
  }
  return false;
}

const Projection& Constraints::projection() {
  return p;
}
