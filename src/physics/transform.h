// -*- C++ -*-

#ifndef MBOSTOCK_TRANSFORM_H
#define MBOSTOCK_TRANSFORM_H

namespace mbostock {

  class Transform {
  public:
    Transform();
    virtual ~Transform() {}

    void enable(bool enabled = true);
    inline bool enabled() const { return enabled_; }

    virtual void reset() = 0;
    virtual void step() = 0;

  private:
    bool enabled_;
  };

}

#endif
