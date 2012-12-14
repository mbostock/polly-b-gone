// -*- C++ -*-

#include <iostream>
#include <stdlib.h>

#include "fan.h"
#include "material.h"
#include "physics/particle.h"
#include "physics/shape.h"

using namespace mbostock;

namespace mbostock {

  class StaticFanModel : public Model {
  public:
    StaticFanModel(float r);
    virtual ~StaticFanModel();

    virtual void initialize();
    virtual void display();

    void setMaterial(const Material& m);

  private:
    void displayBlades();

    const Material* material_;
    AxisAlignedBox bladeBox_;
    AxisAlignedBoxModel bladeModel_;
    GLUquadric* quadric_;
    float r_;
  };

}

StaticFanModel::StaticFanModel(float r)
    : material_(&Materials::blank()),
      bladeBox_(Vector(0.f, 0.f, 0.f),
                Vector(r, r / 10.f, r / 20.f)),
      bladeModel_(bladeBox_), quadric_(NULL), r_(r) {
}

StaticFanModel::~StaticFanModel() {
  if (quadric_ != NULL) {
    gluDeleteQuadric(quadric_);
  }
}

void StaticFanModel::setMaterial(const Material& m) {
  material_ = &m;
  bladeModel_.setMaterial(m);
}

void StaticFanModel::initialize() {
  if (quadric_ != NULL) {
    gluDeleteQuadric(quadric_);
  }
  quadric_ = gluNewQuadric();
  bladeModel_.initialize();
}

void StaticFanModel::display() {
  float axleLength = r_ / 10.f * 1.5;
  float axleRadius = r_ / 20.f;

  material_->bind();
  glPushMatrix();
  glTranslatef(0.f, 0.f, axleLength);
  gluSphere(quadric_, axleRadius, 16, 16);
  glPopMatrix();
  gluCylinder(quadric_, axleRadius, axleRadius, axleLength, 16, 4);
  gluQuadricOrientation(quadric_, GLU_INSIDE);
  gluDisk(quadric_, 0.f, axleRadius, 16, 4);
  gluQuadricOrientation(quadric_, GLU_OUTSIDE);
  displayBlades();
}

void StaticFanModel::displayBlades() {
  int blades = 12;
  for (int i = 0, n = blades; i < n; i++) {
    glPushMatrix();
    glRotatef(i * 360.f / n, 0.f, 0.f, 1.f);
    glRotatef(60.f, 1.f, 0.f, 0.f);
    bladeModel_.display();
    glPopMatrix();
  }
}

FanModel::FanModel(const Fan& fan)
    : fan_(fan), staticModel_(new StaticFanModel(fan.cylinder_.radius())),
      compiledModel_(Models::compile(staticModel_)) {
  for (int i = 0; i < 15; i++) {
    orientation_[i] = 0.f;
  }
  orientation_[15] = 1.f;
}

FanModel::~FanModel() {
  delete compiledModel_;
}

float* FanModel::orientation() {
  const Vector& z = fan_.cylinder_.z();
  const Vector& n = (fabsf(z.z) > .5f) ? Vector::Y() : Vector::Z();
  const Vector& x = n.cross(z);
  const Vector& y = -x.cross(z);
  orientation_[0] = x.x; orientation_[1] = x.y; orientation_[2] = x.z;
  orientation_[4] = y.x; orientation_[5] = y.y; orientation_[6] = y.z;
  orientation_[8] = z.x; orientation_[9] = z.y; orientation_[10] = z.z;
  return orientation_;
}

void FanModel::setMaterial(const Material& m) {
  staticModel_->setMaterial(m);
}

void FanModel::initialize() {
  compiledModel_->initialize();
}

void FanModel::display() {
  glPushMatrix();
  glTranslatev(fan_.cylinder_.x0());
  glMultMatrixf(orientation());
  glRotatef(fan_.a_, 0.f, 0.f, 1.f);
  compiledModel_->display();
  glPopMatrix();
}

Fan::Fan(const Vector& x, const Vector& v, float r, float s)
    : cylinder_(x, x + v * (r / 10.f), r),
      s_(s * ParticleSimulator::timeStep()),
      a_(0.f), model_(*this) {
}

Model& Fan::model() {
  return model_;
}

const Shape& Fan::shape() const {
  return cylinder_;
}

void Fan::step(const ParticleSimulator& s) {
  a_ += s_;
}

void Fan::reset() {
  a_ = 0.f;
}
