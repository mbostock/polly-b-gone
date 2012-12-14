// -*- C++ -*-

#include <stdlib.h>

#include "model.h"
#include "resource.h"
#include "shader.h"

using namespace mbostock;

static Shader* defaultShader_ = NULL;
static Shader* normalShader_ = NULL;
static Shader* wireframeShader_ = NULL;

class DefaultShader : public Shader {
public:
  virtual void display(Model& model) {
    model.display();
  }
};

class WireframeShader : public Shader {
public:
  virtual void display(Model& model) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    model.display();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
};

Shader* Shaders::defaultShader() {
  if (defaultShader_ == NULL) {
    defaultShader_ = new DefaultShader();
  }
  return defaultShader_;
}

Shader* Shaders::wireframeShader() {
  if (wireframeShader_ == NULL) {
    wireframeShader_ = new WireframeShader();
  }
  return wireframeShader_;
}

Shader* Shaders::normalShader() {
  if (normalShader_ == NULL) {
    normalShader_ = new GlslShader("normal.vert", "normal.frag");
  }
  return normalShader_;
}

GlslShader::GlslShader(const char* vertexPath, const char* fragmentPath)
  : vertexPath_(vertexPath), fragmentPath_(fragmentPath),
    program_(GL_NONE) {
}

GlslShader::~GlslShader() {
  if (program_ != GL_NONE) {
    glDeleteProgram(program_);
  }
}

void GlslShader::initialize() {
  if (program_ != GL_NONE) {
    glDeleteProgram(program_);
  }
  program_ = glCreateProgram();
  attach(vertexPath_, GL_VERTEX_SHADER);
  attach(fragmentPath_, GL_FRAGMENT_SHADER);
  link();
}

void GlslShader::attach(const char* path, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  const char* source = Resources::readFile(path);
  glShaderSource(shader, 1, &source, NULL);
  delete[] source;
  glCompileShader(shader);
  glAttachShader(program_, shader);
}

void GlslShader::link() {
  glLinkProgram(program_);
}

void GlslShader::display(Model& model) {
  glDisable(GL_LIGHTING);
  glUseProgram(program_);
  model.display();
  glUseProgram(GL_NONE);
  glEnable(GL_LIGHTING);
}
