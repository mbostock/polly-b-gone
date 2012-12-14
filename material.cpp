// -*- C++ -*-

#include <OpenGL/gl.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "material.h"
#include "texture.h"

using namespace mbostock;

Material::Material()
    : shininess_(0.f), texture_(NULL), slip_(0.f) {
  for (int i = 0; i < 4; i++) {
    ambient_[i] = 0.f;
    diffuse_[i] = 0.f;
    specular_[i] = 0.f;
    emission_[i] = 0.f;
  }
}

void Material::setAmbient(float r, float g, float b, float a) {
  ambient_[0] = r;
  ambient_[1] = g;
  ambient_[2] = b;
  ambient_[3] = a;
}

void Material::setDiffuse(float r, float g, float b, float a) {
  diffuse_[0] = r;
  diffuse_[1] = g;
  diffuse_[2] = b;
  diffuse_[3] = a;
}

void Material::setSpecular(float r, float g, float b, float a) {
  specular_[0] = r;
  specular_[1] = g;
  specular_[2] = b;
  specular_[3] = a;
}

void Material::setEmission(float r, float g, float b, float a) {
  emission_[0] = r;
  emission_[1] = g;
  emission_[2] = b;
  emission_[3] = a;
}

void Material::setShininess(float s) {
  shininess_ = s;
}

void Material::setTexture(const char* path) {
  texture_ = (path == NULL) ? NULL : &(Textures::fromFile(path));
}

void Material::setSlipAngle(float angle) {
  slip_ = cosf(angle * (2.f * M_PI / 360.f));
}

void Material::bind() const {
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_);
  glMaterialfv(GL_FRONT, GL_EMISSION, emission_);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess_);
  if (texture_ == NULL) {
    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
  } else {
    texture_->bind();
  }
}

const Material& Materials::blank() {
  static Material m;
  static bool init = false;
  if (!init) {
    init = true;
    m.setAmbient(.2f, .2f, .1f, .1f);
    m.setDiffuse(.4f, .1f, .1f, 1.f);
    m.setSpecular(.05f, .05f, .05f, .1f);
    m.setShininess(10.f);
  }
  return m;
}
