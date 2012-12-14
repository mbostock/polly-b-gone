// -*- C++ -*-

#ifndef MBOSTOCK_SIMULATION_H
#define MBOSTOCK_SIMULATION_H

#include <stdint.h>

namespace mbostock {

  class Simulation {
  public:
    Simulation(uint32_t timeStepMs);
    virtual ~Simulation() {}

    virtual void togglePaused();
    inline bool paused() const { return paused_; }

    void simulate();

  protected:
    virtual void step() = 0;

  private:
    uint32_t elapsedMillis();

    uint32_t timeStepMs_;
    uint32_t skippedMs_;
    uint32_t lastTimeMs_;
    bool paused_;
  };

}

#endif
