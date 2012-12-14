// -*- C++ -*-

#include "physics/vector.h"
#include "portal.h"

using namespace mbostock;

Portal::Portal(const Vector& min, const Vector& max,
               int room, int origin, bool reset)
    : box_(min, max), room_(room), origin_(origin), reset_(reset) {
}
