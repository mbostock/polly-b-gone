// -*- C++ -*-

#ifndef MBOSTOCK_TRAIL_H
#define MBOSTOCK_TRAIL_H

#include <vector>

#include "model.h"
#include "physics/vector.h"

namespace mbostock {

  class Trail {
  public:
    Trail(const Vector& origin);

    bool add(const Vector& p);
    inline const std::vector<Vector>& points() const { return points_; }

  private:
    std::vector<Vector> points_;
  };

  class TrailModel : public Model {
  public:
    TrailModel(const Trail& trail);

    virtual void display();

  private:
    const Trail& trail_;
  };

}

#endif
