// -*- C++ -*-

#ifndef MBOSTOCK_FAN_H
#define MBOSTOCK_FAN_H

#include "model.h"
#include "physics/shape.h"
#include "room_object.h"

namespace mbostock {

  class Fan;
  class StaticFanModel;

  class FanModel : public Model {
  public:
    FanModel(const Fan& fan);
    virtual ~FanModel();

    virtual void initialize();
    virtual void display();

    void setMaterial(const Material& m);

  private:
    float* orientation();

    const Fan& fan_;
    StaticFanModel* staticModel_;
    Model* compiledModel_;
    float orientation_[16];
  };

  class Fan : public DynamicRoomObject {
  public:
    Fan(const Vector& x, const Vector& v, float r, float s);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual void step(const ParticleSimulator& s);
    virtual void reset();

    inline void setMaterial(const Material& m) { model_.setMaterial(m); }

  private:
    const Cylinder cylinder_;
    const float s_;
    float a_;
    FanModel model_;

    friend class FanModel;
  };
}

#endif
