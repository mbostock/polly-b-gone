// -*- C++ -*-

#include <SDL/sdl.h>

#include "simulation.h"

using namespace mbostock;

/*
 * If for some reason we haven't been able to run the simulation in a long time
 * (e.g., the computer went to sleep for two hours!), then we don't want to run
 * the simulation a zillion times to catch up. Instead we just drop some frames
 * and hope it gets better.
 */
static const uint32_t maxSkippedMs = 500;

Simulation::Simulation(uint32_t timeStepMs)
  : timeStepMs_(timeStepMs), skippedMs_(0), lastTimeMs_(0), paused_(false) {
}

uint32_t Simulation::elapsedMillis() {
  uint32_t currentTimeMs = SDL_GetTicks();
  uint32_t differenceMs = currentTimeMs - lastTimeMs_;
  lastTimeMs_ = currentTimeMs;
  return differenceMs;
}

void Simulation::togglePaused() {
  paused_ = !paused_;
}

void Simulation::simulate() {
  if (paused_ || (lastTimeMs_ == 0)) {
    lastTimeMs_ = SDL_GetTicks();
    return;
  }
  skippedMs_ += elapsedMillis();
  if (skippedMs_ > maxSkippedMs) {
    skippedMs_ = timeStepMs_;
  }
  while (skippedMs_ >= timeStepMs_) {
    step();
    skippedMs_ -= timeStepMs_;
  }
}
