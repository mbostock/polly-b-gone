// -*- C++ -*-

#ifndef MBOSTOCK_ROOM_OBJECT_H
#define MBOSTOCK_ROOM_OBJECT_H

namespace mbostock {

  class Model;
  class ParticleSimulator;
  class Shape;
  class UnaryForce;
  class Vector;

  class RoomObject {
  public:
    virtual Model& model() = 0;
    virtual const Shape& shape() const = 0;
    virtual bool dynamic() const;
    virtual Vector velocity(const Vector& x) const;
    virtual float slip() const;
    virtual void resetForces();
    virtual void applyForce(UnaryForce& force);
    virtual void applyWeight(float w, const Vector& x);
    virtual void step(const ParticleSimulator& s);
    virtual void constrainInternal();
    virtual void reset();
  };

  class DynamicRoomObject : public RoomObject {
  public:
    virtual bool dynamic() const;
  };

}

#endif
