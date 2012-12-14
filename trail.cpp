// -*- C++ -*-

#include "trail.h"

using namespace mbostock;

static const float minsq = .1f * .1f;

Trail::Trail(const Vector& origin) {
  points_.push_back(origin);
}

bool Trail::add(const Vector& p) {
  if ((points_.back() - p).squared() >= minsq) {
    points_.push_back(p);
    return true;
  }
  return false;
}

TrailModel::TrailModel(const Trail& trail)
    : trail_(trail) {
}

void TrailModel::display() {
  glDisable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(.6f, .2f, .3f, .5f);
  glBegin(GL_LINE_STRIP);
  std::vector<Vector>::const_iterator i;
  for (i = trail_.points().begin(); i != trail_.points().end(); i++) {
    glVertexv(*i);
  }
  glEnd();
  glDisable(GL_BLEND);
  glEnable(GL_LIGHTING);
}
