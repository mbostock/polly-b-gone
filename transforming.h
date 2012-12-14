// -*- C++ -*-

#ifndef MBOSTOCK_TRANSFORMING_H
#define MBOSTOCK_TRANSFORMING_H

#include "room_object.h"

namespace mbostock {

  class TransformingRoomObject : public DynamicRoomObject {
  public:
    TransformingRoomObject(RoomObject* o);
    virtual ~TransformingRoomObject();

    virtual float slip() const;
    virtual void resetForces();
    virtual void applyForce(UnaryForce& force);
    virtual void applyWeight(float w, const Vector& x);
    virtual void step(const ParticleSimulator& s);
    virtual void constrainInternal();
    virtual void reset();

  protected:
    RoomObject* object_;
  };

}

#endif
