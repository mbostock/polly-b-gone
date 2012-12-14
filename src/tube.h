// -*- C++ -*-

#ifndef MBOSTOCK_TUBE_H
#define MBOSTOCK_TUBE_H

#include "model.h"
#include "physics/shape.h"
#include "room_object.h"

namespace mbostock {

  class Tube : public RoomObject {
  public:
    Tube(const Vector& x0, const Vector& x1, const Vector& y, float radius);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual float slip() const;

    void setMaterial(const Material& m);
    void setCapMaterial(const Material& m);

  private:
    const Cylinder cylinder_;
    Vector y_;
    CylinderModel model_;
  };

}

#endif
