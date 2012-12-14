// -*- C++ -*-

#ifndef MBOSTOCK_TRANSLATING_H
#define MBOSTOCK_TRANSLATING_H

#include "model.h"
#include "physics/shape.h"
#include "physics/translation.h"
#include "physics/vector.h"
#include "room_object.h"
#include "transforming.h"

namespace mbostock {

  class TranslatingModel : public Model {
  public:
    TranslatingModel(Model& m, const Translation& t);

    virtual void initialize();
    virtual void display();

  private:
    Model& model_;
    const Translation& translation_;
  };

  class TranslatingRoomObject : public TransformingRoomObject {
  public:
    TranslatingRoomObject(RoomObject* o, const Translation& t);

    virtual Model& model();
    virtual const Shape& shape() const;
    virtual Vector velocity(const Vector& x) const;

  private:
    const Translation& translation_;
    TranslatingShape shape_;
    TranslatingModel model_;
  };

}

#endif
