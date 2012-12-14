// -*- C++ -*-

#ifndef MBOSTOCK_ESCALATOR_H
#define MBOSTOCK_ESCALATOR_H

#include "model.h"
#include "physics/shape.h"
#include "physics/vector.h"
#include "room_object.h"

namespace mbostock {

  class Material;
  class ParticleSimulator;

  class Escalator : public DynamicRoomObject {
  public:
    Escalator(const Vector& min, const Vector& max, const Vector& v);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual void step(const ParticleSimulator& s);
    virtual Vector velocity(const Vector& x) const;
    virtual float slip() const;

    void setMaterial(const Material& m);
    void setTopMaterial(const Material& m);

  private:
    const AxisAlignedBox box_;
    const Vector velocity_;
    Vector offset_;

    AxisAlignedBoxModel model_;
  };

}

#endif
