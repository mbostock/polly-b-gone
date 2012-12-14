// -*- C++ -*-

#include <OpenGL/gl.h>

#include "material.h"
#include "portal.h"
#include "room.h"
#include "room_force.h"
#include "room_object.h"
#include "sound.h"
#include "trail.h"
#include "world.h"

using namespace mbostock;

/* Fog. */
static const float fogDensity = 0.02f;
static const float fogColor[] = { 0.f, 0.f, 0.f, 1.f };

static const float gravity = 10.f;
static const float minY = -50.f;

static World* world_ = NULL;

WorldModel::WorldModel(World& world)
    : world_(world) {
}

void WorldModel::initialize() {
  glFogi(GL_FOG_MODE, GL_EXP2);
  glFogfv(GL_FOG_COLOR, fogColor);
  glFogf(GL_FOG_DENSITY, fogDensity);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_FOG);
  glEnable(GL_LIGHTING);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_TEXTURE_2D);

  std::vector<Room*>::const_iterator i;
  for (i = world_.rooms().begin(); i != world_.rooms().end(); i++) {
    (*i)->model().initialize();
  }
  world_.player().model().initialize();
  world_.pauseLighting().initialize();
}

void WorldModel::display() {
  world_.room().model().display();

  if (world_.debug()) {
    std::vector<Trail*>::const_iterator i;
    for (i = world_.room().trails().begin(); i != world_.room().trails().end();
         i++) {
      TrailModel m(**i);
      m.initialize();
      m.display();
    }
    TrailModel m(world_.room().trail());
    m.initialize();
    m.display();
  }

  world_.player().model().display();
}

World::World()
    : Simulation(roundf(ParticleSimulator::timeStep() * 1000.f)),
      simulator_(1.f), gravity_(gravity), room_(NULL), debug_(false),
      model_(*this) {
  world_ = this;
  pauseLighting_.light(0).setDiffuse(.1f, .1f, .1f, 1.f);
  pauseLighting_.light(0).setSpecular(.1f, .1f, .1f, 1.f);
}

World::~World() {
  std::vector<Room*>::const_iterator ir;
  for (ir = rooms_.begin(); ir != rooms_.end(); ir++) {
    delete (*ir);
  }
  std::vector<Material*>::const_iterator im;
  for (im = materials_.begin(); im != materials_.end(); im++) {
    delete (*im);
  }
}

World* World::world() {
  return world_;
}

void World::addRoom(Room* r) {
  if (room_ == NULL) {
    setRoom(r, r->origins()[0]);
  }
  rooms_.push_back(r);
}

void World::addMaterial(Material* m) {
  materials_.push_back(m);
}

void World::addLighting(Lighting* l) {
  lightings_.push_back(l);
}

void World::togglePaused() {
  Simulation::togglePaused();
  if (paused()) {
    Sounds::pause();
  } else {
    Sounds::resume();
  }
}

void World::toggleDebug() {
  debug_ = !debug_;
}

void World::setRoom(Room* r, RoomOrigin* origin) {
  bool sameMusic = false;
  if ((room_ != NULL) && (room_->music() != NULL)) {
    if (room_->music() == r->music()) {
      sameMusic = true;
    } else {
      room_->music()->stop();
    }
  }

  room_ = r;
  room_->nextTrail(origin->position());
  player_.setOrigin(origin->position());
  player_.setVelocity(origin->velocity());

  if (!sameMusic && (room_->music() != NULL)) {
    room_->music()->play(-1);
  }
}

void World::step() {
  std::vector<RoomObject*>::const_iterator i;

  /* Reset forces. */
  player_.resetForces();
  room_->resetForces();

  /* Apply gravity and contact forces. */
  player_.applyForce(gravity_);
  room_->applyForce(gravity_);
  for (i = contactObjects_.begin(); i != contactObjects_.end(); i++) {
    (*i)->applyWeight(gravity * player_.mass(), player_.origin());
  }

  /* Apply localized forces. */
  std::vector<RoomForce*>::const_iterator f;
  for (f = room_->forces().begin(); f != room_->forces().end(); f++) {
    RoomForce& force = **f;
    if (player_.intersects(force.shape())) {
      player_.applyForce(force);
    }
  }

  /* Run the simulation. */
  player_.step(simulator_);
  room_->step(simulator_);

  /* If the player landed on a portal, move to the associated room. */
  std::vector<Portal*>::const_iterator p;
  for (p = room_->portals().begin(); p != room_->portals().end(); p++) {
    Portal& portal = **p;
    if (portal.contains(player_.origin())) {
      if (portal.reset()) {
        room_->reset();
      }
      Room* room = rooms_[portal.room()];
      setRoom(room, room->origins()[portal.origin()]);
      contactObjects_.clear();
      return;
    }
  }

  /* Apply constraints, detect contacts. */
  contactObjects_.clear();
  for (i = room_->objects().begin(); i != room_->objects().end(); i++) {
    RoomObject& object = **i;
    object.constrainInternal();
    if (player_.constrainOutside(object)) {
      contactObjects_.push_back(&object);
    }
  }
  player_.constrainInternal();
  room_->trail().add(player_.origin());

  /* Reset if the player falls into a chasm. */
  if (player_.origin().y < minY) {
    resetPlayer();
  }
}

void World::resetPlayer() {
  RoomOrigin* origin = room_->origins()[0];
  player_.setOrigin(origin->position());
  player_.setVelocity(origin->velocity());
  room_->reset();
  room_->nextTrail(origin->position());
}

void World::nextRoom() {
  std::vector<Room*>::const_iterator i;
  Room* nextRoom = rooms_[0];
  for (i = rooms_.begin(); i != rooms_.end(); i++) {
    if ((*i) == room_) {
      i++;
      if (i != rooms_.end()) {
        nextRoom = *i;
      }
      break;
    }
  }
  setRoom(nextRoom, nextRoom->origins()[0]);
}

void World::previousRoom() {
  std::vector<Room*>::reverse_iterator i;
  Room* nextRoom = rooms_[rooms_.size() - 1];
  for (i = rooms_.rbegin(); i != rooms_.rend(); i++) {
    if ((*i) == room_) {
      i++;
      if (i != rooms_.rend()) {
        nextRoom = *i;
      }
      break;
    }
  }
  setRoom(nextRoom, nextRoom->origins()[0]);
}
