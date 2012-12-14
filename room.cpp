// -*- C++ -*-

#include <algorithm>
#include <iostream>

#include "lighting.h"
#include "physics/transform.h"
#include "physics/vector.h"
#include "portal.h"
#include "room.h"
#include "room_force.h"
#include "room_object.h"
#include "sound.h"
#include "trail.h"
#include "world.h"

using namespace mbostock;

class RoomStaticModel : public Model {
public:
  RoomStaticModel(const Room& room) : room_(room) {}

  virtual void initialize() {
    staticObjects_.clear();
    std::vector<RoomObject*>::const_iterator i;
    for (i = room_.objects().begin(); i != room_.objects().end(); i++) {
      RoomObject* o = *i;
      if (!o->dynamic()) {
        o->model().initialize();
        staticObjects_.push_back(o);
      }
    }
  }

  virtual void display() {
    std::vector<RoomObject*>::const_iterator i;
    for (i = staticObjects_.begin(); i != staticObjects_.end(); i++) {
      (*i)->model().display();
    }
  }

private:
  const Room& room_;
  std::vector<RoomObject*> staticObjects_;
};

RoomModel::RoomModel(const Room& room)
    : room_(room), staticModel_(Models::compile(new RoomStaticModel(room))) {
}

RoomModel::~RoomModel() {
  delete staticModel_;
}

void RoomModel::initialize() {
  room_.lighting().initialize();
  staticModel_->initialize();
  dynamicObjects_.clear();
  std::vector<RoomObject*>::const_iterator i;
  for (i = room_.objects().begin(); i != room_.objects().end(); i++) {
    RoomObject* o = *i;
    if (o->dynamic()) {
      o->model().initialize();
      dynamicObjects_.push_back(o);
    }
  }
}

void RoomModel::display() {
  if (World::world()->paused()) {
    World::world()->pauseLighting().display();
  } else {
    room_.lighting().display();
  }
  staticModel_->display();
  std::vector<RoomObject*>::const_iterator i;
  for (i = dynamicObjects_.begin(); i != dynamicObjects_.end(); i++) {
    (*i)->model().display();
  }
}

Room::Room()
    : lighting_(&(Lightings::standard())), music_(NULL),
      cameraBounds_(-Vector::INF(), Vector::INF()),
      model_(*this), trail_(NULL) {
}

Room::~Room() {
  std::vector<RoomObject*>::const_iterator io;
  for (io = objects_.begin(); io != objects_.end(); io++) {
    delete (*io);
  }
  std::vector<RoomOrigin*>::const_iterator ir;
  for (ir = origins_.begin(); ir != origins_.end(); ir++) {
    delete (*ir);
  }
  std::vector<Portal*>::const_iterator ip;
  for (ip = portals_.begin(); ip != portals_.end(); ip++) {
    delete (*ip);
  }
  std::vector<RoomForce*>::const_iterator ii;
  for (ii = forces_.begin(); ii != forces_.end(); ii++) {
    delete (*ii);
  }
  std::vector<Transform*>::const_iterator iz;
  for (iz = transforms_.begin(); iz != transforms_.end(); iz++) {
    delete (*iz);
  }
  std::vector<Trail*>::const_iterator it;
  for (it = trails_.begin(); it != trails_.end(); it++) {
    delete (*it);
  }
  if (trail_ != NULL) {
    delete trail_;
  }
}

void Room::nextTrail(const Vector& origin) {
  if (trail_ != NULL) {
    trails_.push_back(trail_);
  }
  trail_ = new Trail(origin);
}

void Room::setMusic(const Sound& music) {
  music_ = &music;
}

void Room::setLighting(const Lighting& lighting) {
  lighting_ = &lighting;
}

void Room::setCameraBounds(const Vector& min, const Vector& max) {
  cameraBounds_ = AxisAlignedBox(min, max);
}

void Room::resetForces() {
  std::vector<RoomObject*>::const_iterator i;
  for (i = objects_.begin(); i != objects_.end(); i++) {
    (*i)->resetForces();
  }
}

void Room::applyForce(UnaryForce& force) {
  std::vector<RoomObject*>::const_iterator i;
  for (i = objects_.begin(); i != objects_.end(); i++) {
    (*i)->applyForce(force);
  }
}

void Room::step(const ParticleSimulator& s) {
  std::vector<Transform*>::const_iterator ir;
  for (ir = transforms_.begin(); ir != transforms_.end(); ir++) {
    (*ir)->step();
  }
  std::vector<RoomObject*>::const_iterator i;
  for (i = objects_.begin(); i != objects_.end(); i++) {
    (*i)->step(s);
  }
}

void Room::reset() {
  std::vector<RoomObject*>::const_iterator i;
  for (i = objects_.begin(); i != objects_.end(); i++) {
    (*i)->reset();
  }
  std::vector<Transform*>::const_iterator iz;
  for (iz = transforms_.begin(); iz != transforms_.end(); iz++) {
    (*iz)->reset();
  }
}

RoomOrigin::RoomOrigin(const Vector& position, const Vector& velocity)
    : position_(position), velocity_(velocity) {
}
