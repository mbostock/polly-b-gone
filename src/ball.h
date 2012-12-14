// -*- C++ -*-

#ifndef MBOSTOCK_BALL_H
#define MBOSTOCK_BALL_H

#include "model.h"
#include "physics/shape.h"
#include "room_object.h"

namespace mbostock {

  class Ball : public RoomObject {
  public:
    Ball(const Vector& x, float radius);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual float slip() const;

    inline void setMaterial(const Material& m) { model_.setMaterial(m); }

  private:
    const Sphere sphere_;
    SphereModel model_;
  };

}


#endif
