// -*- C++ -*-

#include "material.h"
#include "physics/transform.h"
#include "physics/vector.h"
#include "switch.h"

using namespace mbostock;

Switch::Switch(const Vector& min, const Vector& max)
    : AxisAlignedBlock(min, max), activeMaterial_(NULL) {
}

bool Switch::dynamic() const {
  return (activeMaterial_ != NULL);
}

void Switch::addTarget(Transform& t) {
  t.enable(false);
  targets_.push_back(&t);
}

void Switch::setActiveMaterial(const Material& m) {
  activeMaterial_ = &m;
  inactiveMaterial_ = &model_.material();
  inactiveTopMaterial_ = &model_.topMaterial();
}

void Switch::reset() {
  if (activeMaterial_ != NULL) {
    setMaterial(*inactiveMaterial_);
    setTopMaterial(*inactiveTopMaterial_);
  }
  std::vector<Transform*>::const_iterator i;
  for (i = targets_.begin(); i != targets_.end(); i++) {
    (*i)->enable(false);
  }
}

void Switch::applyWeight(float w, const Vector& x) {
  if (activeMaterial_ != NULL) {
    setMaterial(*activeMaterial_);
    setTopMaterial(*activeMaterial_);
  }
  std::vector<Transform*>::const_iterator i;
  for (i = targets_.begin(); i != targets_.end(); i++) {
    (*i)->enable();
  }
}
