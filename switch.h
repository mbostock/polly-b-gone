// -*- C++ -*-

#ifndef MBOSTOCK_SWITCH_H
#define MBOSTOCK_SWITCH_H

#include <vector>

#include "block.h"

namespace mbostock {

  class Transform;

  class Switch : public AxisAlignedBlock {
  public:
    Switch(const Vector& min, const Vector& max);

    virtual bool dynamic() const;
    virtual void applyWeight(float w, const Vector& x);
    virtual void reset();

    void addTarget(Transform& t);
    void setActiveMaterial(const Material& m);

  private:
    std::vector<Transform*> targets_;
    const Material* inactiveMaterial_;
    const Material* inactiveTopMaterial_;
    const Material* activeMaterial_;
  };

}

#endif
