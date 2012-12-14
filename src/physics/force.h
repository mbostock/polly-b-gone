// -*- C++ -*-

#ifndef MBOSTOCK_FORCE_H
#define MBOSTOCK_FORCE_H

#include "vector.h"

namespace mbostock {

  class Particle;

  class UnaryForce {
  public:
    virtual ~UnaryForce() {}

    void apply(Particle& p);

    virtual Vector force(const Particle& p)  = 0;
  };

  class BinaryForce {
  public:
    virtual ~BinaryForce() {}

    void apply(Particle& a, Particle& b);

    virtual Vector force(const Particle& a, const Particle& b) = 0;
  };

  class RandomForce : public UnaryForce {
  public:
    RandomForce(float k);

    virtual Vector force(Particle& p);

  private:
    float k_;
  };

  class GravitationalForce : public UnaryForce {
  public:
    GravitationalForce(float g);

    virtual Vector force(const Particle& p);

  private:
    float g_;
  };

  class LinearDragForce : public UnaryForce {
  public:
    LinearDragForce(float kd);

    virtual Vector force(const Particle& p);

  private:
    float kd_;
  };

  class QuadraticDragForce : public UnaryForce {
  public:
    QuadraticDragForce(float kd);

    virtual Vector force(const Particle& p);

  private:
    float kd_;
  };

  class HookeForce : public BinaryForce {
  public:
    HookeForce(float r, float ks);

    virtual Vector force(const Particle& a, const Particle& b);

  private:
    float r_;
    float ks_;
  };

  class DampenedHookeForce : public BinaryForce {
  public:
    DampenedHookeForce(float r, float ks, float kd);

    virtual Vector force(const Particle& a, const Particle& b);

  private:
    float r_;
    float ks_;
    float kd_;
  };

  class CoulombForce : public BinaryForce {
  public:
    CoulombForce(float e);

    virtual Vector force(const Particle& a, const Particle& b);

  private:
    float e_;
  };

}

#endif
