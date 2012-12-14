// -*- C++ -*-

#ifndef MBOSTOCK_ROTATION_H
#define MBOSTOCK_ROTATION_H

#include "shape.h"
#include "vector.h"
#include "transform.h"

namespace mbostock {

  /**
   * Computes a rotation matrix for a given origin, axis and angle. This is
   * roughly the equivalent to a glTranslatef and glRotatef call in OpenGL.
   */
  class Rotation : public Transform {
  public:
    Rotation(const Vector& origin, const Vector& axis,
             float speed, float angle);

    /** Rotates the specified point. */
    Vector point(const Vector& x) const;

    /** Inverse-rotates the specified point. */
    Vector pointInverse(const Vector& x) const;

    /** Rotates the specified vector. */
    Vector vector(const Vector& x) const;

    /** Inverse-rotates the specified vector. */
    Vector vectorInverse(const Vector& x) const;

    /** Returns the rotation's origin. */
    inline const Vector& origin() const { return origin_; }

    /** Returns the rotation's axis. */
    inline const Vector& axis() const { return axis_; }

    /** Returns the rotation's angle (in degrees, in the range [0, 360]). */
    inline float angle() const { return angle_; }

    /** Returns the speed of the rotation (in degrees per time step). */
    inline float speed() const { return speed_; }

    /** Advances the rotation by one time step. */
    virtual void step();

    /** Resets the rotation. */
    virtual void reset();

    /** Returns the velocity of the given point. */
    Vector velocity(const Vector& x) const;

  private:
    void update();

    Vector origin_;
    Vector axis_;
    float startAngle_;
    float angle_;
    float speed_;
    float matrix_[9];
  };

  class RotatingShape : public Shape {
  public:
    RotatingShape(const Shape& s, const Rotation& r);

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& x) const;

  private:
    const Shape& shape_;
    const Rotation& rotation_;
  };

}

#endif
