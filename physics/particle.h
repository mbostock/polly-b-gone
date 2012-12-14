// -*- C++ -*-

#ifndef MBOSTOCK_PARTICLE_H
#define MBOSTOCK_PARTICLE_H

#include "vector.h"

namespace mbostock {

  class Particle {
  public:
    Particle();

    float inverseMass;
    Vector position;
    Vector previousPosition;
    Vector force;

    Vector velocity() const;
  };

  class ParticleSimulator {
  public:
    ParticleSimulator();
    ParticleSimulator(float drag);

    void step(Particle& p) const;

    static float timeStep();

  private:
    static float timeStep_;
    static float timeStepSquared_;
    float drag_;
  };

}

#endif
