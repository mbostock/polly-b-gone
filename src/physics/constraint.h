// -*- C++ -*-

#ifndef MBOSTOCK_CONSTRAINT_H
#define MBOSTOCK_CONSTRAINT_H

#include "shape.h"
#include "vector.h"

namespace mbostock {

  class Particle;
  class Shape;

  class Constraints {
  public:

    /**
     * Constrains a particle a to be distance d from position p, independent of
     * mass.
     */
    static bool distance(Particle& a, Vector p, float d);

    /**
     * Constrains two particles a and b to be distance d apart. The particles
     * are moved in opposite directions according to their relative mass, either
     * towards or away from each other, depending on whether their actual
     * distance is greater or less than the specified distance d.
     */
    static bool distance(Particle& a, Particle& b, float d);

    /**
     * Constrains a particle a to be at least distance d from position p,
     * independent of mass.
     */
    static bool minDistance(Particle& a, Vector p, float d);

    /**
     * Constrains two particles a and b to be at least distance d apart. The
     * particles are moved in opposite directions according to their relative
     * mass, away from each other, if their actual distance is less than the
     * specified distance d.
     */
    static bool minDistance(Particle& a, Particle& b, float d);

    /**
     * Constrains a particle a to be at most distance d from position p,
     * independent of mass.
     */
    static bool maxDistance(Particle& a, Vector p, float d);

    /**
     * Constrains two particles a and b to be at most distance d apart. The
     * particles are moved in opposite directions according to their relative
     * mass, towards each other, if their actual distance is greater than the
     * specified distance d.
     */
    static bool maxDistance(Particle& a, Particle& b, float d);

    /**
     * Constrains the particle so that its y-value is at least min. This
     * implicitly uses a coefficient of restitution of zero.
     */
    static bool minY(Particle& a, float min) {
      return plane(a, Vector(0, min, 0), Vector(0, 1, 0));
    }

    /**
     * Constrains the particle so that its y-value is at least min, simulating a
     * collision with a plane of coefficient of restitution kr. Note that the
     * bounce can only be simulated if the particle was previously above the
     * plane.
     */
    static bool minY(Particle& a, float min, float kr) {
      return plane(a, Vector(0, min, 0), Vector(0, 1, 0), kr);
    }

    /**
     * Constrains the particle to be above the plane defined by the point p and
     * the normal n. This implicitly uses a coefficient of restitution of zero.
     */
    static bool plane(Particle& a, Vector p, Vector n);

    /**
     * Constrains the particle to be above the plane defined by the point p and
     * the normal n, using a coefficient of restitution kr.
     */
    static bool plane(Particle& a, Vector p, Vector n, float kr);

    /**
     * Constrains the particle to be inside the specified shape by at least
     * radius r. This implicitly uses a coefficient of restitution of zero.
     */
    static bool inside(Particle& a, const Shape& s, float r);

    /**
     * Constrains the particle to be inside the specified shape by at least
     * radius r, using a coefficient of restitution kr.
     */
    static bool inside(Particle& a, const Shape& s, float r, float kr);

    /**
     * Constrains the particle to be outside the specified shape by at least
     * radius r. This implicitly uses a coefficient of restitution of zero.
     */
    static bool outside(Particle& a, const Shape& s, float r);

    /**
     * Constrains the particle to be outside the specified shape by at least
     * radius r, using a coefficient of restitution kr.
     */
    static bool outside(Particle& a, const Shape& s, float r, float kr);

    /**
     * Returns the projection information for the last call to a shape-based
     * constraint.
     */
    static const Projection& projection();

  private:
    Constraints();
  };

}

#endif
