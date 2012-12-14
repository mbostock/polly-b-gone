// -*- C++ -*-

#ifndef MBOSTOCK_BLOCK_H
#define MBOSTOCK_BLOCK_H

#include "model.h"
#include "physics/shape.h"
#include "room_object.h"

namespace mbostock {

  class AxisAlignedBlock : public RoomObject {
  public:
    AxisAlignedBlock(const Vector& min, const Vector& max);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual float slip() const;

    void setMaterial(const Material& m);
    void setTopMaterial(const Material& m);

  protected:
    const AxisAlignedBox box_;
    AxisAlignedBoxModel model_;
  };

  class Block : public RoomObject {
  public:
    Block(const Vector& c, const Vector& x, const Vector& y, const Vector& z);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual float slip() const;

    void setMaterial(const Material& m);
    void setTopMaterial(const Material& m);

  private:
    const Box box_;
    BoxModel model_;
  };

}


#endif
