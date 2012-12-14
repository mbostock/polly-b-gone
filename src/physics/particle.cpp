// -*- C++ -*-

#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

using namespace mbostock;

Particle::Particle() : inverseMass(1.f) {
}

Vector Particle::velocity() const {
  /* Note: ignores force * (inverseMass * timeStep / 2). */
  return (position - previousPosition) / ParticleSimulator::timeStep();
}

ParticleSimulator::ParticleSimulator()
    : drag_(1.f) {
}

ParticleSimulator::ParticleSimulator(float drag) 
    : drag_(drag) {
}

float ParticleSimulator::timeStep() {
  return .003f; // TODO .005 and interpolate
}

void ParticleSimulator::step(Particle& p) const {
  static const float timeStepSquared = timeStep() * timeStep();

  Vector p0 = p.previousPosition;
  p.previousPosition = p.position;
  p.position += (p.position - p0) * drag_
      + p.force * (p.inverseMass * timeStepSquared);
}
