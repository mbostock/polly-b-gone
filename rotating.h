// -*- C++ -*-

#ifndef MBOSTOCK_ROTATING_H
#define MBOSTOCK_ROTATING_H

#include "model.h"
#include "physics/rotation.h"
#include "physics/vector.h"
#include "room_force.h"
#include "room_object.h"
#include "transforming.h"

namespace mbostock {

  class RotatingModel : public Model {
  public:
    RotatingModel(Model& m, const Rotation& r);

    virtual void initialize();
    virtual void display();

  private:
    Model& model_;
    const Rotation& rotation_;
  };

  class RotatingRoomObject : public TransformingRoomObject {
  public:
    RotatingRoomObject(RoomObject* o, const Rotation& r);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual Vector velocity(const Vector& x) const;

  private:
    const Rotation& rotation_;
    RotatingShape shape_;
    RotatingModel model_;
  };

  class RotatingRoomForce : public RoomForce {
  public:
    RotatingRoomForce(RoomForce* f, const Rotation& r);
    virtual ~RotatingRoomForce();

    virtual const Shape& shape() const;
    virtual Vector force(const Particle& p);

  private:
    RoomForce* force_;
    const Rotation& rotation_;
    RotatingShape shape_;
  };

}

#endif
