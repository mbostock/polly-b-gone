// -*- C++ -*-

#include "material.h"
#include "model.h"
#include "physics/shape.h"
#include "room.h"
#include "wall.h"

using namespace mbostock;

Wall::Wall(const Vector& x0, const Vector& x1,
           const Vector& x2, const Vector& x3)
    : quad_(x0, x1, x2, x3), model_(quad_) {
}

Model& Wall::model() {
  return model_;
}

const Shape& Wall::shape() const {
  return quad_;
}

void Wall::setMaterial(const Material& m) {
  model_.setMaterial(m);
}

float Wall::slip() const {
  return model_.material().slip();
}

void Wall::setTexCoords(const Vector& t0, const Vector& t1,
                        const Vector& t2, const Vector& t3) {
  model_.setTexCoords(t0, t1, t2, t3);
}

TriWall::TriWall(const Vector& x0, const Vector& x1, const Vector& x2)
    : triangle_(x0, x1, x2), model_(triangle_) {
}

Model& TriWall::model() {
  return model_;
}

const Shape& TriWall::shape() const {
  return triangle_;
}

void TriWall::setTexCoords(const Vector& t0, const Vector& t1,
                           const Vector& t2) {
  model_.setTexCoords(t0, t1, t2);
}

void TriWall::setMaterial(const Material& m) {
  model_.setMaterial(m);
}

float TriWall::slip() const {
  return model_.material().slip();
}
