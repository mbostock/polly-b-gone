// -*- C++ -*-

#ifndef MBOSTOCK_ROOM_FORCE_H
#define MBOSTOCK_ROOM_FORCE_H

#include "physics/force.h"
#include "physics/shape.h"
#include "physics/vector.h"

namespace mbostock {

  class Particle;
  class Shape;

  /**
   * Defines a force that applies to the player when the player intersects a
   * given shape. For example, this can be used to represent a fan blowing on
   * the player, which the force of the fan stronger near the fan blades,
   * falling off to zero outside of the fan's stream.
   */
  class RoomForce : public UnaryForce {
  public:
    virtual const Shape& shape() const = 0;
  };

  /**
   * A simple room force that applies a constant force within an axis-aligned
   * box. More elaborate forces can be approximated by composing multiple (even
   * overlapping) constant room forces.
   */
  class ConstantRoomForce : public RoomForce {
  public:
    ConstantRoomForce(const Vector& min, const Vector& max, const Vector& f);

    virtual const Shape& shape() const;
    virtual Vector force(const Particle& p);

  private:
    AxisAlignedBox box_;
    Vector force_;
  };

}

#endif
