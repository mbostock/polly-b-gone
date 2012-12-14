// -*- C++ -*-

#ifndef _SHADER_H
#define _SHADER_H

#include <OpenGL/gl.h>

namespace mbostock {

  class Model;

  class Shader {
  public:
    virtual ~Shader() {};

    virtual void initialize() {}
    virtual void display(Model& model) = 0;
  };

  class GlslShader : public Shader {
  public:
    GlslShader(const char* vertexPath, const char* fragmentPath);
    virtual ~GlslShader();

    virtual void initialize();
    virtual void display(Model& model);

  private:
    void attach(const char* path, GLenum shaderType);
    void link();

    const char* vertexPath_;
    const char* fragmentPath_;
    GLuint program_;
  };

  class Shaders {
  public:
    static Shader* defaultShader();
    static Shader* normalShader();
    static Shader* wireframeShader();

  private:
    Shaders();
  };

}

#endif
