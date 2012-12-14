// -*- C++ -*-

#include <stdlib.h>

#include "lighting.h"

using namespace mbostock;

Light::Light()
  : id_(-1), enabled_(false) {
  setAmbient(0.f, 0.f, 0.f, 1.f);
  setDiffuse(0.f, 0.f, 0.f, 1.f);
  setSpecular(0.f, 0.f, 0.f, 1.f);
  setPosition(0.f, 0.f, 1.f, 0.f);
  setSpotDirection(0.f, 0.f, -1.f);
  spotExponent_ = 0.f;
  spotCutoff_ = 180.f;
  constantAttenuation_ = 1.f;
  linearAttenuation_ = 0.f;
  quadraticAttenuation_ = 0.f;
}

void Light::setAmbient(float r, float g, float b, float a) {
  ambient_[0] = r;
  ambient_[1] = g;
  ambient_[2] = b;
  ambient_[3] = a;
}

void Light::setDiffuse(float r, float g, float b, float a) {
  diffuse_[0] = r;
  diffuse_[1] = g;
  diffuse_[2] = b;
  diffuse_[3] = a;
}

void Light::setSpecular(float r, float g, float b, float a) {
  specular_[0] = r;
  specular_[1] = g;
  specular_[2] = b;
  specular_[3] = a;
}

void Light::setPosition(float x, float y, float z, float w) {
  position_[0] = x;
  position_[1] = y;
  position_[2] = z;
  position_[3] = w;
}

void Light::setSpotDirection(float x, float y, float z) {
  spotDirection_[0] = x;
  spotDirection_[1] = y;
  spotDirection_[2] = z;
}

void Light::setSpotExponent(float e) {
  spotExponent_ = e;
}

void Light::setConstantAttenuation(float a) {
  constantAttenuation_ = a;
}

void Light::setLinearAttenuation(float a) {
  linearAttenuation_ = a;
}

void Light::setQuadraticAttenuation(float a) {
  quadraticAttenuation_ = a;
}

void Light::initialize() const {
  if (enabled_) {
    glEnable(id_);
    glLightfv(id_, GL_AMBIENT, ambient_);
    glLightfv(id_, GL_DIFFUSE, diffuse_);
    glLightfv(id_, GL_SPECULAR, specular_);
    glLightf(id_, GL_SPOT_EXPONENT, spotExponent_);
    glLightf(id_, GL_SPOT_CUTOFF, spotCutoff_);
    glLightf(id_, GL_CONSTANT_ATTENUATION, constantAttenuation_);
    glLightf(id_, GL_LINEAR_ATTENUATION, linearAttenuation_);
    glLightf(id_, GL_QUADRATIC_ATTENUATION, quadraticAttenuation_);
  } else {
    glDisable(id_);
  }
}

void Light::display() const {
  if (enabled_) {
    glLightfv(id_, GL_POSITION, position_);
    glLightfv(id_, GL_SPOT_DIRECTION, spotDirection_);
  }
}

void Light::enable() {
  enabled_ = true;
}

void Light::disable() {
  enabled_ = false;
}

Lighting::Lighting() {
  for (int i = 0; i < lights(); i++) {
    lights_[i].id_ = GL_LIGHT0 + i;
  }
  setGlobalAmbient(.2f, .2f, .2f, 1.f);
  lights_[0].setDiffuse(1.f, 1.f, 1.f, 1.f);
  lights_[0].setSpecular(1.f, 1.f, 1.f, 1.f);
  lights_[0].enable();
}

void Lighting::setGlobalAmbient(float r, float g, float b, float a) {
  globalAmbient_[0] = r;
  globalAmbient_[1] = g;
  globalAmbient_[2] = b;
  globalAmbient_[3] = a;
}

static const Lighting* lastLighting_ = NULL;

void Lighting::initialize() const {
  lastLighting_ = this;
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient_);
  for (int i = 0; i < lights(); i++) {
    lights_[i].initialize();
  }
}

void Lighting::display() const {
  if (lastLighting_ != this) {
    lastLighting_ = this;
    initialize();
  }
  for (int i = 0; i < lights(); i++) {
    lights_[i].display();
  }
}

const Lighting& Lightings::standard() {
  static const Lighting l;
  return l;
}
