// -*- C++ -*-

#ifndef MBOSTOCK_ROOM_H
#define MBOSTOCK_ROOM_H

#include <vector>

#include "model.h"

namespace mbostock {

  class Lighting;
  class ParticleSimulator;
  class Portal;
  class Room;
  class RoomForce;
  class RoomObject;
  class RoomOrigin;
  class Shape;
  class Sound;
  class Trail;
  class Transform;
  class UnaryForce;

  class RoomModel : public Model {
  public:
    RoomModel(const Room& room);
    virtual ~RoomModel();

    virtual void initialize();
    virtual void display();

  private:
    const Room& room_;
    std::vector<RoomObject*> dynamicObjects_;
    Model* staticModel_;
  };

  class Room {
  public:
    Room();
    ~Room();

    inline void addForce(RoomForce* o) { forces_.push_back(o); }
    inline void addOrigin(RoomOrigin* o) { origins_.push_back(o); }
    inline void addObject(RoomObject* o) { objects_.push_back(o); }
    inline void addPortal(Portal* p) { portals_.push_back(p); }
    inline void addTransform(Transform* r) { transforms_.push_back(r); }

    inline const std::vector<RoomForce*>& forces() const { return forces_; }
    inline const std::vector<RoomOrigin*>& origins() const { return origins_; }
    inline const std::vector<RoomObject*>& objects() const { return objects_; }
    inline const std::vector<Portal*>& portals() const { return portals_; }
    inline const std::vector<Trail*>& trails() const { return trails_; }

    inline Model& model() { return model_; }
    inline Trail& trail() { return *trail_; }
    inline const Lighting& lighting() const { return *lighting_; }
    inline const AxisAlignedBox& cameraBounds() const { return cameraBounds_; }
    inline const Sound* music() const { return music_; }

    void setMusic(const Sound& music);
    void setLighting(const Lighting& lighting);
    void setCameraBounds(const Vector& min, const Vector& max);
    void resetForces();
    void applyForce(UnaryForce& force);
    void step(const ParticleSimulator& s);
    void reset();
    void nextTrail(const Vector& origin);

  private:
    std::vector<RoomForce*> forces_;
    std::vector<RoomOrigin*> origins_;
    std::vector<RoomObject*> objects_;
    std::vector<Portal*> portals_;
    std::vector<Trail*> trails_;
    std::vector<Transform*> transforms_;
    const Lighting* lighting_;
    const Sound* music_;
    AxisAlignedBox cameraBounds_;
    RoomModel model_;
    Trail* trail_;
  };

  class RoomOrigin {
  public:
    RoomOrigin(const Vector& position, const Vector& velocity);

    inline const Vector& position() const { return position_; }
    inline const Vector& velocity() const { return velocity_; }

  private:
    Vector position_;
    Vector velocity_;
  };

}

#endif
