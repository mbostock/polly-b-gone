// -*- C++ -*-

#ifndef MBOSTOCK_MATERIAL_H
#define MBOSTOCK_MATERIAL_H

namespace mbostock {

  class Texture;

  class Material {
  public:
    Material();

    void setAmbient(float r, float g, float b, float a);
    void setDiffuse(float r, float g, float b, float a);
    void setSpecular(float r, float g, float b, float a);
    void setEmission(float r, float g, float b, float a);
    void setShininess(float s);
    void setTexture(const char* path);

    void setSlipAngle(float angle);
    inline float slip() const { return slip_; }

    void bind() const;

  private:
    float ambient_[4];
    float diffuse_[4];
    float specular_[4];
    float emission_[4];
    float shininess_;
    const Texture* texture_;

    float slip_;
  };

  class Materials {
  public:
    static const Material& blank();

  private:
    Materials();
  };

}

#endif
