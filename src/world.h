// -*- C++ -*-

#ifndef MBOSTOCK_WORLD_H
#define MBOSTOCK_WORLD_H

#include <vector>

#include "lighting.h"
#include "model.h"
#include "physics/force.h"
#include "player.h"
#include "simulation.h"

namespace mbostock {

  class Material;
  class Room;
  class RoomObject;
  class RoomOrigin;
  class World;

  class WorldModel : public Model {
  public:
    WorldModel(World& world);

    virtual void initialize();
    virtual void display();

  private:
    World& world_;
  };

  class World : public Simulation {
  public:
    World();
    virtual ~World();

    static World* world();

    void addRoom(Room* r);
    void addMaterial(Material* m);
    void addLighting(Lighting* l);

    inline Player& player() { return player_; }
    inline const std::vector<Room*>& rooms() const { return rooms_; }
    inline Room& room() const { return *room_; }
    inline Model& model() { return model_; }
    inline const Lighting& pauseLighting() const { return pauseLighting_; }

    void resetPlayer();
    void nextRoom();
    void previousRoom();

    virtual void togglePaused();

    void toggleDebug();
    inline bool debug() const { return debug_; }

  protected:
    virtual void step();

  private:
    void setRoom(Room* room, RoomOrigin* origin);

    ParticleSimulator simulator_;
    GravitationalForce gravity_;
    Player player_;
    Lighting pauseLighting_;
    std::vector<Lighting*> lightings_;
    std::vector<Material*> materials_;
    std::vector<Room*> rooms_;
    std::vector<RoomObject*> contactObjects_;
    Room* room_;
    bool debug_;
    WorldModel model_;
  };

}

#endif
