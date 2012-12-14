// -*- C++ -*-

#include "block.h"
#include "material.h"
#include "model.h"
#include "physics/shape.h"
#include "physics/vector.h"
#include "room.h"

using namespace mbostock;

AxisAlignedBlock::AxisAlignedBlock(const Vector& min, const Vector& max)
    : box_(min, max), model_(box_) {
}

Model& AxisAlignedBlock::model() {
  return model_;
}

const Shape& AxisAlignedBlock::shape() const {
  return box_;
}

void AxisAlignedBlock::setMaterial(const Material& m) {
  model_.setMaterial(m);
}

void AxisAlignedBlock::setTopMaterial(const Material& m) {
  model_.setTopMaterial(m);
}

float AxisAlignedBlock::slip() const {
  return model_.material().slip();
}

Block::Block(const Vector& c, const Vector& x, const Vector& y, const Vector& z)
    : box_(c, x, y, z), model_(box_) {
}

Model& Block::model() {
  return model_;
}

const Shape& Block::shape() const {
  return box_;
}

void Block::setMaterial(const Material& m) {
  model_.setMaterial(m);
}

void Block::setTopMaterial(const Material& m) {
  model_.setTopMaterial(m);
}

float Block::slip() const {
  return model_.material().slip();
}
