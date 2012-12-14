// -*- C++ -*-

#ifndef MBOSTOCK_WORLDS_H
#define MBOSTOCK_WORLDS_H

namespace mbostock {

  class World;

  class Worlds {
  public:
    static World* fromFile(const char* path);

  private:
    Worlds();
  };

}

#endif
