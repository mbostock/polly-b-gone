// -*- C++ -*-

#ifndef MBOSTOCK_WALL_H
#define MBOSTOCK_WALL_H

#include "model.h"
#include "physics/shape.h"
#include "room_object.h"

namespace mbostock {

  class Material;

  class Wall : public RoomObject {
  public:
    Wall(const Vector& x0, const Vector& x1,
         const Vector& x2, const Vector& x3);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual float slip() const;

    void setTexCoords(const Vector& t0, const Vector& t1,
                      const Vector& t2, const Vector& t3);
    void setMaterial(const Material& m);

  private:
    const Quad quad_;
    QuadModel model_;
  };

  class TriWall : public RoomObject {
  public:
    TriWall(const Vector& x0, const Vector& x1, const Vector& x2);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual float slip() const;

    void setTexCoords(const Vector& t0, const Vector& t1, const Vector& t2);
    void setMaterial(const Material& m);

  private:
    const Triangle triangle_;
    TriangleModel model_;
  };

}


#endif
