// -*- C++ -*-

#ifndef MBOSTOCK_SEESAW_H
#define MBOSTOCK_SEESAW_H

#include "model.h"
#include "physics/force.h"
#include "physics/particle.h"
#include "physics/shape.h"
#include "physics/vector.h"
#include "room_object.h"

namespace mbostock {

  class Material;

  class Seesaw : public DynamicRoomObject {
  public:
    Seesaw(const Vector& min, const Vector& max, float mass);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual void resetForces();
    virtual void applyForce(UnaryForce& force);
    virtual void applyWeight(float w, const Vector& x);
    virtual void step(const ParticleSimulator& s);
    virtual void constrainInternal();
    virtual void reset();
    virtual float slip() const;

    void setMaterial(const Material& m);
    void setTopMaterial(const Material& m);

  private:
    void updateBox();

    const Vector origin_;
    const Vector size_;
    LinearDragForce drag_;

    Box box_;
    Particle left_;
    Particle right_;
    Particle center_;

    BoxModel model_;
  };

}

#endif
