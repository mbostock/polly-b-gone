// -*- C++ -*-

#include <fstream>
#include <ios>
#include <iostream>
#include <string>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <mach-o/dyld.h>
#include <libgen.h>
#endif

#include "resource.h"

using namespace mbostock;

static std::string resourcePath;

static void initResourcePath() {
  if (!resourcePath.empty()) return;

#ifdef __APPLE__
  // Try to get the bundle resources path
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  if (mainBundle) {
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    if (resourcesURL) {
      char path[PATH_MAX];
      if (CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8*)path, PATH_MAX)) {
        resourcePath = path;
        resourcePath += "/";
        CFRelease(resourcesURL);
        return;
      }
      CFRelease(resourcesURL);
    }
  }

  // Fallback: get executable path and look for resources relative to it
  char execPath[PATH_MAX];
  uint32_t size = sizeof(execPath);
  if (_NSGetExecutablePath(execPath, &size) == 0) {
    char* dir = dirname(execPath);
    resourcePath = dir;
    resourcePath += "/../Resources/";
    return;
  }
#endif

  // Last fallback
  resourcePath = "Contents/Resources/";
}

const char* Resources::path() {
  initResourcePath();
  return resourcePath.c_str();
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
