// -*- C++ -*-

#ifndef MBOSTOCK_TEXTURE_H
#define MBOSTOCK_TEXTURE_H

#include <OpenGL/gl.h>

namespace mbostock {

  class Texture {
  public:
    virtual ~Texture() {}

    virtual void bind() const = 0;
  };

  class Textures {
  public:
    static const Texture& fromFile(const char* path);
    static void initialize();

  private:
    Textures();
  };

}

#endif
