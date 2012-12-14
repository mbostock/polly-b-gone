// -*- C++ -*-

#ifndef MBOSTOCK_PORTAL_H
#define MBOSTOCK_PORTAL_H

#include "physics/shape.h"

namespace mbostock {

  class Vector;

  class Portal {
  public:
    Portal(const Vector& min, const Vector& max,
           int room, int origin, bool reset);

    inline bool contains(const Vector& p) const { return box_.contains(p); }
    inline const AxisAlignedBox& bounds() const { return box_; }
    inline int room() const { return room_; }
    inline int origin() const { return origin_; }
    inline bool reset() const { return reset_; }

  private:
    AxisAlignedBox box_;
    int room_;
    int origin_;
    bool reset_;
  };

}


#endif
