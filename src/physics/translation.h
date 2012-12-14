// -*- C++ -*-

#ifndef MBOSTOCK_TRANSLATION_H
#define MBOSTOCK_TRANSLATION_H

#include "shape.h"
#include "transform.h"
#include "vector.h"

namespace mbostock {

  class Translation : public Transform {
  public:
    Translation(const Vector& x0, const Vector& x1,
                float s, float u, float kd);

    enum Mode { REVERSE, RESET, ONE_WAY };

    /* Sets the translation mode. The default is REVERSE. */
    void setMode(Mode m);

    /** Translates the specified point. */
    Vector point(const Vector& x) const;

    /** Inverse-translates the specified point. */
    Vector pointInverse(const Vector& x) const;

    /** Advances the translation by one time step. */
    virtual void step();

    /** Resets the translation. */
    virtual void reset();

    /** Returns the current velocity. */
    const Vector& velocity() const;

    /** Returns the current origin. */
    inline const Vector& origin() const { return origin_; }

  private:
    void update();

    Vector x0_;
    Vector x1_;
    float s_;
    float u_;
    float kd_;
    Vector v_;
    Vector dv_;
    Vector x_;
    Vector origin_;
    Mode mode_;
    bool reversed_;
  };

  class TranslatingShape : public Shape {
  public:
    TranslatingShape(const Shape& s, const Translation& t);

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& x) const;

  private:
    const Shape& shape_;
    const Translation& translation_;
  };

}

#endif
