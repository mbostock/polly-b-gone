// -*- C++ -*-

#ifndef MBOSTOCK_LIGHTING_H
#define MBOSTOCK_LIGHTING_H

#include <OpenGL/gl.h>

namespace mbostock {

  class Light {
  public:
    Light();

    void initialize() const;
    void display() const;

    void enable();
    void disable();
    inline bool enabled() const { return enabled_; }

    void setAmbient(float r, float g, float b, float a);
    void setDiffuse(float r, float g, float b, float a);
    void setSpecular(float r, float g, float b, float a);
    void setPosition(float x, float y, float z, float w);
    void setSpotDirection(float x, float y, float z);
    void setSpotExponent(float e);
    void setConstantAttenuation(float a);
    void setLinearAttenuation(float a);
    void setQuadraticAttenuation(float a);

  private:
    GLenum id_;
    bool enabled_;
    float ambient_[4];
    float diffuse_[4];
    float specular_[4];
    float position_[4];
    float spotDirection_[3];
    float spotExponent_;
    float spotCutoff_;
    float constantAttenuation_;
    float linearAttenuation_;
    float quadraticAttenuation_;

    friend class Lighting;
  };

  class Lighting {
  public:
    Lighting();

    void initialize() const;
    void display() const;

    void setGlobalAmbient(float r, float g, float b, float a);
    inline Light& light(int i) { return lights_[i]; }
    inline int lights() const { return 8; }

  private:
    float globalAmbient_[4];
    Light lights_[8];
  };

  class Lightings {
  public:
    static const Lighting& standard();

  private:
    Lightings();
  };

}

#endif
