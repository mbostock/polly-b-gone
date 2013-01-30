// -*- C++ -*-

#include <fstream>
#include <ios>
#include <iostream>

#include "resource.h"

using namespace mbostock;

const char* Resources::path() {
#ifdef __APPLE__
   return "Contents/Resources/";
#else
  return "resources/";
#endif
}

const char* Resources::readFile(const char* p) {
  std::string fullPath(path());
  fullPath.append(p);
  std::ifstream file(fullPath.c_str());
  file.seekg(0, std::ios::end);
  std::ifstream::pos_type size = file.tellg();
  file.seekg(0, std::ios::beg);
  char* buffer = new char[1 + size];
  file.read(buffer, size);
  buffer[size] = '\0';
  file.close();
  return buffer;
}
