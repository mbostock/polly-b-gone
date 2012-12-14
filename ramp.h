// -*- C++ -*-

#ifndef MBOSTOCK_RAMP_H
#define MBOSTOCK_RAMP_H

#include "model.h"
#include "physics/shape.h"
#include "room_object.h"

namespace mbostock {

  class Ramp : public RoomObject {
  public:
    Ramp(const Vector& x0, const Vector& x1,
         const Vector& x2, const Vector& x3);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual float slip() const;

    void setMaterial(const Material& m);
    void setTopMaterial(const Material& m);

  private:
    const Wedge wedge_;
    WedgeModel model_;
  };

}


#endif
