// -*- C++ -*-

#ifndef MBOSTOCK_RESOURCE_H
#define MBOSTOCK_RESOURCE_H

namespace mbostock {

  class Resources {
  public:
    static const char* path();
    static const char* readFile(const char* path);

  private:
    Resources();
  };

}

#endif
