#include <TinyXML/tinyxml.h>
#include <iostream>
#include <list>
#include <map>
#include <math.h>
#include <string>
#include <vector>

#include "ball.h"
#include "block.h"
#include "escalator.h"
#include "fan.h"
#include "lighting.h"
#include "material.h"
#include "portal.h"
#include "ramp.h"
#include "resource.h"
#include "room.h"
#include "room_force.h"
#include "rotating.h"
#include "seesaw.h"
#include "sound.h"
#include "switch.h"
#include "translating.h"
#include "tube.h"
#include "wall.h"
#include "world.h"
#include "worlds.h"

namespace mbostock {

  class Portal;
  class RoomForce;
  class RoomObject;
  class RoomOrigin;
  class World;

  class SwitchTargets {
  public:
    SwitchTargets(Switch* s);

    inline Switch* svitch() const { return switch_; }

    void addTarget(const char* targetName);
    inline const std::vector<std::string>& targets() const { return targets_; }

  private:
    Switch* switch_;
    std::vector<std::string> targets_;
  };

  class XmlWorldBuilder {
  public:
    XmlWorldBuilder();

    World* parseWorld(const char* path);

  private:
    static Vector parseVector(TiXmlElement* e, const Vector& d);
    static Vector parseVector(TiXmlElement* e);
    static bool parseBool(TiXmlElement* e, const char* name, bool d);
    static bool parseBool(TiXmlElement* e, const char* name);

    void parseLightings(TiXmlElement* e);
    void parseLighting(TiXmlElement* e);
    void parseLightGlobalAmbient(Lighting* l, TiXmlElement* e);
    void parseLights(Lighting* l, TiXmlElement* e);
    void parseLight(Light& l, TiXmlElement* e);
    void parseLightAmbient(Light& l, TiXmlElement* e);
    void parseLightDiffuse(Light& l, TiXmlElement* e);
    void parseLightSpecular(Light& l, TiXmlElement* e);
    void parseLightPosition(Light& l, TiXmlElement* e);
    void parseLightSpotDirection(Light& l, TiXmlElement* e);
    void parseLightAttributes(Light& l, TiXmlElement* e);

    void parseMaterials(TiXmlElement* e);
    void parseMaterial(TiXmlElement* e);
    void parseMaterialParameter(Material* m, TiXmlElement* e);

    void parseRooms(TiXmlElement* e);
    void parseRoom(TiXmlElement* e);
    void parseRoomLighting(Room* r, TiXmlElement* e);
    void parseRoomMusic(Room* r, TiXmlElement* e);
    void parseRoomCameraBounds(Room* r, TiXmlElement* e);
    void parseRoomTopLevelObjects(Room* r, TiXmlElement* e);
    void parseRoomObjects(Room* r, TiXmlElement* e);

    RoomOrigin* parseRoomOrigin(TiXmlElement* e);
    Portal* parseRoomPortal(TiXmlElement* e);

    RoomObject* parseRoomObject(TiXmlElement* e);
    RoomObject* parseRoomBlock(TiXmlElement* e);
    RoomObject* parseRoomAxisAlignedBlock(TiXmlElement* e);
    RoomObject* parseRoomOrientedBlock(TiXmlElement* e);
    RoomObject* parseRoomWall(TiXmlElement* e);
    RoomObject* parseRoomQuadWall(TiXmlElement* e);
    RoomObject* parseRoomTriWall(TiXmlElement* e);
    RoomObject* parseRoomEscalator(TiXmlElement* e);
    RoomObject* parseRoomSeesaw(TiXmlElement* e);
    RoomObject* parseRoomRamp(TiXmlElement* e);
    RoomObject* parseRoomTube(TiXmlElement* e);
    RoomObject* parseRoomBall(TiXmlElement* e);
    RoomObject* parseRoomFan(TiXmlElement* e);
    RoomObject* parseRoomSwitch(TiXmlElement* e);

    RoomForce* parseRoomConstantForce(TiXmlElement* e);

    void parseRotation(Room* r, TiXmlElement* e);
    void parseTranslation(Room* r, TiXmlElement* e);
    RoomObject* applyTransforms(RoomObject* o);

    void parseRoomSwitchTargets(Switch* s, TiXmlElement* e);
    void resolveSwitchTargets();

    int findRoom(const char* name);
    int findRoomOrigin(const char* name);

    TiXmlDocument document_;
    World* world_;
    std::map<std::string, Lighting*> lightings_;
    std::map<std::string, Material*> materials_;
    std::map<std::string, Transform*> transforms_;
    std::list<Transform*> activeTransforms_;
    std::vector<SwitchTargets*> switchTargets_;
  };

}

using namespace mbostock;

SwitchTargets::SwitchTargets(Switch* s)
    : switch_(s) {
}

void SwitchTargets::addTarget(const char* targetName) {
  targets_.push_back(targetName);
}

XmlWorldBuilder::XmlWorldBuilder()
    : world_(NULL) {
}

World* XmlWorldBuilder::parseWorld(const char* path) {
  std::string fullPath(Resources::path());
  fullPath.append(path);
  if (!document_.LoadFile(fullPath.c_str())) {
    std::cerr << "Error loading world \"" << path << "\": ";
    std::cerr << document_.ErrorDesc() << "\n";
    return NULL;
  }

  world_ = new World();
  TiXmlElement* e = document_.FirstChildElement("world");
  parseLightings(e);
  parseMaterials(e);
  parseRooms(e);
  resolveSwitchTargets();
  return world_;
}

void XmlWorldBuilder::parseLightings(TiXmlElement* e) {
  for (TiXmlElement* l = e->FirstChildElement("lighting"); l != NULL;
       l = l->NextSiblingElement("lighting")) {
    parseLighting(l);
  }
}

void XmlWorldBuilder::parseLighting(TiXmlElement* e) {
  Lighting* l = new Lighting();
  lightings_[e->Attribute("name")] = l;
  parseLightGlobalAmbient(l, e->FirstChildElement("ambient"));
  parseLights(l, e);
  world_->addLighting(l);
}

void XmlWorldBuilder::parseLightGlobalAmbient(Lighting* l, TiXmlElement* e) {
  if (e != NULL) {
    float r = 0.f, g = 0.f, b = 0.f, a = 1.f;
    e->QueryFloatAttribute("r", &r);
    e->QueryFloatAttribute("g", &g);
    e->QueryFloatAttribute("b", &b);
    e->QueryFloatAttribute("a", &a);
    l->setGlobalAmbient(r, g, b, a);
  }
}

void XmlWorldBuilder::parseLights(Lighting* l, TiXmlElement* e) {
  int i = 0;
  for (TiXmlElement* le = e->FirstChildElement("light");
       le != NULL; le = le->NextSiblingElement("light")) {
    parseLight(l->light(i++), le);
  }
}

void XmlWorldBuilder::parseLight(Light& l, TiXmlElement* e) {
  parseLightAmbient(l, e->FirstChildElement("ambient"));
  parseLightSpecular(l, e->FirstChildElement("specular"));
  parseLightDiffuse(l, e->FirstChildElement("diffuse"));
  parseLightPosition(l, e->FirstChildElement("position"));
  parseLightSpotDirection(l, e->FirstChildElement("spot-direction"));
  parseLightAttributes(l, e);
  l.enable();
}

void XmlWorldBuilder::parseLightAmbient(Light& l, TiXmlElement* e) {
  if (e != NULL) {
    float r = 0.f, g = 0.f, b = 0.f, a = 1.f;
    e->QueryFloatAttribute("r", &r);
    e->QueryFloatAttribute("g", &g);
    e->QueryFloatAttribute("b", &b);
    e->QueryFloatAttribute("a", &a);
    l.setAmbient(r, g, b, a);
  }
}

void XmlWorldBuilder::parseLightSpecular(Light& l, TiXmlElement* e) {
  if (e != NULL) {
    float r = 0.f, g = 0.f, b = 0.f, a = 1.f;
    e->QueryFloatAttribute("r", &r);
    e->QueryFloatAttribute("g", &g);
    e->QueryFloatAttribute("b", &b);
    e->QueryFloatAttribute("a", &a);
    l.setSpecular(r, g, b, a);
  }
}

void XmlWorldBuilder::parseLightDiffuse(Light& l, TiXmlElement* e) {
  if (e != NULL) {
    float r = 0.f, g = 0.f, b = 0.f, a = 1.f;
    e->QueryFloatAttribute("r", &r);
    e->QueryFloatAttribute("g", &g);
    e->QueryFloatAttribute("b", &b);
    e->QueryFloatAttribute("a", &a);
    l.setDiffuse(r, g, b, a);
  }
}

void XmlWorldBuilder::parseLightPosition(Light& l, TiXmlElement* e) {
  if (e != NULL) {
    float x, y, z, w;
    e->QueryFloatAttribute("x", &x);
    e->QueryFloatAttribute("y", &y);
    e->QueryFloatAttribute("z", &z);
    e->QueryFloatAttribute("w", &w);
    l.setPosition(x, y, z, w);
  }
}

void XmlWorldBuilder::parseLightSpotDirection(Light& l, TiXmlElement* e) {
  if (e != NULL) {
    float x, y, z;
    e->QueryFloatAttribute("x", &x);
    e->QueryFloatAttribute("y", &y);
    e->QueryFloatAttribute("z", &z);
    l.setSpotDirection(x, y, z);
  }
}

void XmlWorldBuilder::parseLightAttributes(Light& l, TiXmlElement* e) {
  if (e->Attribute("spot-exponent")) {
    float f;
    e->QueryFloatAttribute("spot-exponent", &f);
    l.setSpotExponent(f);
  }
  if (e->Attribute("constant-attenuation")) {
    float f;
    e->QueryFloatAttribute("constant-attenuation", &f);
    l.setConstantAttenuation(f);
  }
  if (e->Attribute("linear-attenuation")) {
    float f;
    e->QueryFloatAttribute("linear-attenuation", &f);
    l.setLinearAttenuation(f);
  }
  if (e->Attribute("quadratic-attenuation")) {
    float f;
    e->QueryFloatAttribute("quadratic-attenuation", &f);
    l.setQuadraticAttenuation(f);
  }
}

void XmlWorldBuilder::parseMaterials(TiXmlElement* e) {
  for (TiXmlElement* m = e->FirstChildElement("material"); m != NULL;
       m = m->NextSiblingElement("material")) {
    parseMaterial(m);
  }
}

void XmlWorldBuilder::parseMaterial(TiXmlElement* e) {
  Material* m = new Material();
  materials_[e->Attribute("name")] = m;
  float s = 90.f;
  e->QueryFloatAttribute("slip-angle", &s);
  m->setSlipAngle(s);
  for (TiXmlElement* p = e->FirstChildElement(); p != NULL;
       p = p->NextSiblingElement()) {
    parseMaterialParameter(m, p);
  }
  world_->addMaterial(m);
}

void XmlWorldBuilder::parseMaterialParameter(Material* m, TiXmlElement* e) {
  const std::string& name = e->ValueStr();
  if (name == "texture") {
    m->setTexture(e->Attribute("path"));
    return;
  }

  float r = 0.f, g = 0.f, b = 0.f, a = 1.f;
  e->QueryFloatAttribute("r", &r);
  e->QueryFloatAttribute("g", &g);
  e->QueryFloatAttribute("b", &b);
  e->QueryFloatAttribute("a", &a);

  if (name == "ambient") {
    m->setAmbient(r, g, b, a);
  } else if (name == "diffuse") {
    m->setDiffuse(r, g, b, a);
  } else if (name == "emission") {
    m->setEmission(r, g, b, a);
  } else if (name == "specular") {
    m->setSpecular(r, g, b, a);
  }
}

void XmlWorldBuilder::parseRooms(TiXmlElement* e) {
  for (TiXmlElement* r = e->FirstChildElement("room"); r != NULL;
       r = r->NextSiblingElement("room")) {
    parseRoom(r);
  }
}

void XmlWorldBuilder::parseRoom(TiXmlElement* e) {
  Room* r = new Room();
  parseRoomLighting(r, e);
  parseRoomMusic(r, e);
  parseRoomCameraBounds(r, e);
  parseRoomTopLevelObjects(r, e);
  world_->addRoom(r);
}

void XmlWorldBuilder::parseRoomLighting(Room* r, TiXmlElement* e) {
  const char* lighting = e->Attribute("lighting");
  if (lighting != NULL) {
    r->setLighting(*lightings_[lighting]);
  }
}

void XmlWorldBuilder::parseRoomMusic(Room* r, TiXmlElement* e) {
  const char* music = e->Attribute("music");
  if (music != NULL) {
    r->setMusic(Sounds::fromFile(music));
  }
}

void XmlWorldBuilder::parseRoomCameraBounds(Room* r, TiXmlElement* e) {
  Vector min = parseVector(e->FirstChildElement("camera-min"), -Vector::INF());
  Vector max = parseVector(e->FirstChildElement("camera-max"), Vector::INF());
  r->setCameraBounds(min, max);
}

void XmlWorldBuilder::parseRoomTopLevelObjects(Room* r, TiXmlElement* e) {
  for (TiXmlElement* o = e->FirstChildElement(); o != NULL;
       o = o->NextSiblingElement()) {
    const std::string& name = o->ValueStr();
    if (name == "origin") {
      r->addOrigin(parseRoomOrigin(o));
    } else if (name == "portal") {
      r->addPortal(parseRoomPortal(o));
    }
  }
  parseRoomObjects(r, e);
}

void XmlWorldBuilder::parseRoomObjects(Room* r, TiXmlElement* e) {
  for (TiXmlElement* o = e->FirstChildElement(); o != NULL;
       o = o->NextSiblingElement()) {
    const std::string& name = o->ValueStr();
    if (name == "constant-force") {
      r->addForce(parseRoomConstantForce(o));
    } else if (name == "rotation") {
      parseRotation(r, o);
    } else if (name == "translation") {
      parseTranslation(r, o);
    } else {
      RoomObject* ro = parseRoomObject(o);
      if (ro != NULL) { // ignore unknown objects
        r->addObject(applyTransforms(ro));
      }
    }
  }
}

void XmlWorldBuilder::parseRotation(Room* r, TiXmlElement* e) {
  float s = 10.f, a = 0.f;
  e->QueryFloatAttribute("speed", &s);
  e->QueryFloatAttribute("angle", &a);
  Rotation* z = new Rotation(
      parseVector(e->FirstChildElement("origin")),
      parseVector(e->FirstChildElement("axis")),
      s, a);
  const char* name = e->Attribute("name");
  if (name != NULL) {
    transforms_[name] = z;
  }
  activeTransforms_.push_back(z);
  parseRoomObjects(r, e);
  activeTransforms_.pop_back();
  r->addTransform(z);
}

void XmlWorldBuilder::parseTranslation(Room* r, TiXmlElement* e) {
  float s = 10.f, u = 0.f, kd = 0.f;
  e->QueryFloatAttribute("speed", &s);
  e->QueryFloatAttribute("start", &u);
  e->QueryFloatAttribute("dampen", &kd);
  Translation* z = new Translation(
      parseVector(e->FirstChildElement("x0")),
      parseVector(e->FirstChildElement("x1")),
      s, u, kd);
  const char* mode = e->Attribute("mode");
  if (mode != NULL) {
    std::string modeString = mode;
    if (modeString == "one-way") {
      z->setMode(Translation::ONE_WAY);
    } else if (modeString == "reset") {
      z->setMode(Translation::RESET);
    }
  }
  const char* name = e->Attribute("name");
  if (name != NULL) {
    transforms_[name] = z;
  }
  activeTransforms_.push_back(z);
  parseRoomObjects(r, e);
  activeTransforms_.pop_back();
  r->addTransform(z);
}

RoomObject* XmlWorldBuilder::applyTransforms(RoomObject* o) {
  std::list<Transform*>::reverse_iterator i;
  for (i = activeTransforms_.rbegin(); i != activeTransforms_.rend(); i++) {
    Transform* z = *i;
    Translation* t = dynamic_cast<Translation*>(z);
    if (t != NULL) {
      o = new TranslatingRoomObject(o, *t);
      continue;
    }
    Rotation* r = dynamic_cast<Rotation*>(z);
    if (r != NULL) {
      o = new RotatingRoomObject(o, *r);
      continue;
    }
  }
  return o;
}

RoomOrigin* XmlWorldBuilder::parseRoomOrigin(TiXmlElement* e) {
  return new RoomOrigin(
      parseVector(e->FirstChildElement("position")),
      parseVector(e->FirstChildElement("velocity")));
}

Portal* XmlWorldBuilder::parseRoomPortal(TiXmlElement* e) {
  return new Portal(
      parseVector(e->FirstChildElement("min")),
      parseVector(e->FirstChildElement("max")),
      findRoom(e->Attribute("origin")),
      findRoomOrigin(e->Attribute("origin")),
      parseBool(e, "reset", false));
}

RoomObject* XmlWorldBuilder::parseRoomObject(TiXmlElement* e) {
  const std::string& name = e->ValueStr();
  if (name == "block") {
    return parseRoomBlock(e);
  } else if (name == "wall") {
    return parseRoomWall(e);
  } else if (name == "escalator") {
    return parseRoomEscalator(e);
  } else if (name == "seesaw") {
    return parseRoomSeesaw(e);
  } else if (name == "ramp") {
    return parseRoomRamp(e);
  } else if (name == "tube") {
    return parseRoomTube(e);
  } else if (name == "ball") {
    return parseRoomBall(e);
  } else if (name == "fan") {
    return parseRoomFan(e);
  } else if (name == "switch") {
    return parseRoomSwitch(e);
  }
  return NULL;
}

RoomObject* XmlWorldBuilder::parseRoomBlock(TiXmlElement* e) {
  return (e->FirstChildElement("min") != NULL)
      ? parseRoomAxisAlignedBlock(e)
      : parseRoomOrientedBlock(e);
}

RoomObject* XmlWorldBuilder::parseRoomAxisAlignedBlock(TiXmlElement* e) {
  AxisAlignedBlock* b = new AxisAlignedBlock(
      parseVector(e->FirstChildElement("min")),
      parseVector(e->FirstChildElement("max")));
  const char* material = e->Attribute("material");
  if (material != NULL) {
    b->setMaterial(*materials_[material]);
  }
  const char* topMaterial = e->Attribute("top-material");
  if (topMaterial != NULL) {
    b->setTopMaterial(*materials_[topMaterial]);
  }
  return b;
}

RoomObject* XmlWorldBuilder::parseRoomOrientedBlock(TiXmlElement* e) {
  Block* b = new Block(
      parseVector(e->FirstChildElement("c")),
      parseVector(e->FirstChildElement("x")),
      parseVector(e->FirstChildElement("y")),
      parseVector(e->FirstChildElement("z")));
  const char* material = e->Attribute("material");
  if (material != NULL) {
    b->setMaterial(*materials_[material]);
  }
  const char* topMaterial = e->Attribute("top-material");
  if (topMaterial != NULL) {
    b->setTopMaterial(*materials_[topMaterial]);
  }
  return b;
}

RoomObject* XmlWorldBuilder::parseRoomWall(TiXmlElement* e) {
  return (e->FirstChildElement("x3") == NULL)
      ? parseRoomTriWall(e)
      : parseRoomQuadWall(e);
}

RoomObject* XmlWorldBuilder::parseRoomQuadWall(TiXmlElement* e) {
  Wall* w = new Wall(
      parseVector(e->FirstChildElement("x0")),
      parseVector(e->FirstChildElement("x1")),
      parseVector(e->FirstChildElement("x2")),
      parseVector(e->FirstChildElement("x3")));
  TiXmlElement* t = e->FirstChildElement("tex-coords");
  if (t != NULL) {
    w->setTexCoords(
        parseVector(t->FirstChildElement("t0")),
        parseVector(t->FirstChildElement("t1")),
        parseVector(t->FirstChildElement("t2")),
        parseVector(t->FirstChildElement("t3")));
  }
  const char* material = e->Attribute("material");
  if (material != NULL) {
    w->setMaterial(*materials_[material]);
  }
  return w;
}

RoomObject* XmlWorldBuilder::parseRoomTriWall(TiXmlElement* e) {
  TriWall* w = new TriWall(
      parseVector(e->FirstChildElement("x0")),
      parseVector(e->FirstChildElement("x1")),
      parseVector(e->FirstChildElement("x2")));
  TiXmlElement* t = e->FirstChildElement("tex-coords");
  if (t != NULL) {
    w->setTexCoords(
        parseVector(t->FirstChildElement("t0")),
        parseVector(t->FirstChildElement("t1")),
        parseVector(t->FirstChildElement("t2")));
  }
  const char* material = e->Attribute("material");
  if (material != NULL) {
    w->setMaterial(*materials_[material]);
  }
  return w;
}

RoomObject* XmlWorldBuilder::parseRoomEscalator(TiXmlElement* e) {
  Escalator* o = new Escalator(
      parseVector(e->FirstChildElement("min")),
      parseVector(e->FirstChildElement("max")),
      parseVector(e->FirstChildElement("v")));
  const char* material = e->Attribute("material");
  if (material != NULL) {
    o->setMaterial(*materials_[material]);
  }
  const char* topMaterial = e->Attribute("top-material");
  if (topMaterial != NULL) {
    o->setTopMaterial(*materials_[topMaterial]);
  }
  return o;
}

RoomObject* XmlWorldBuilder::parseRoomSeesaw(TiXmlElement* e) {
  float mass = 1.f;
  e->QueryFloatAttribute("mass", &mass);
  Seesaw* s = new Seesaw(
      parseVector(e->FirstChildElement("min")),
      parseVector(e->FirstChildElement("max")),
      mass);
  const char* material = e->Attribute("material");
  if (material != NULL) {
    s->setMaterial(*materials_[material]);
  }
  const char* topMaterial = e->Attribute("top-material");
  if (topMaterial != NULL) {
    s->setTopMaterial(*materials_[topMaterial]);
  }
  return s;
}

RoomObject* XmlWorldBuilder::parseRoomRamp(TiXmlElement* e) {
  Ramp* r = new Ramp(
      parseVector(e->FirstChildElement("x0")),
      parseVector(e->FirstChildElement("x1")),
      parseVector(e->FirstChildElement("x2")),
      parseVector(e->FirstChildElement("x3")));
  const char* material = e->Attribute("material");
  if (material != NULL) {
    r->setMaterial(*materials_[material]);
  }
  const char* topMaterial = e->Attribute("top-material");
  if (topMaterial != NULL) {
    r->setTopMaterial(*materials_[topMaterial]);
  }
  return r;
}

RoomObject* XmlWorldBuilder::parseRoomTube(TiXmlElement* e) {
  float r = 1.f;
  e->QueryFloatAttribute("radius", &r);
  Tube* t = new Tube(
      parseVector(e->FirstChildElement("x0")),
      parseVector(e->FirstChildElement("x1")),
      parseVector(e->FirstChildElement("y"), Vector::Y()),
      r);
  const char* material = e->Attribute("material");
  if (material != NULL) {
    t->setMaterial(*materials_[material]);
  }
  const char* capMaterial = e->Attribute("cap-material");
  if (capMaterial != NULL) {
    t->setCapMaterial(*materials_[capMaterial]);
  }
  return t;
}

RoomObject* XmlWorldBuilder::parseRoomBall(TiXmlElement* e) {
  float r = 1.f;
  e->QueryFloatAttribute("radius", &r);
  Ball* b = new Ball(parseVector(e->FirstChildElement("x")), r);
  const char* material = e->Attribute("material");
  if (material != NULL) {
    b->setMaterial(*materials_[material]);
  }
  return b;
}

RoomObject* XmlWorldBuilder::parseRoomFan(TiXmlElement* e) {
  float r, s;
  e->QueryFloatAttribute("radius", &r);
  e->QueryFloatAttribute("speed", &s);
  Fan* f = new Fan(
      parseVector(e->FirstChildElement("x")),
      parseVector(e->FirstChildElement("v")),
      r, s);
  const char* material = e->Attribute("material");
  if (material != NULL) {
    f->setMaterial(*materials_[material]);
  }
  return f;
}

RoomObject* XmlWorldBuilder::parseRoomSwitch(TiXmlElement* e) {
  Switch* b = new Switch(
      parseVector(e->FirstChildElement("min")),
      parseVector(e->FirstChildElement("max")));
  const char* material = e->Attribute("material");
  if (material != NULL) {
    b->setMaterial(*materials_[material]);
  }
  const char* topMaterial = e->Attribute("top-material");
  if (topMaterial != NULL) {
    b->setTopMaterial(*materials_[topMaterial]);
  }
  const char* activeMaterial = e->Attribute("active-material");
  if (activeMaterial != NULL) {
    b->setActiveMaterial(*materials_[activeMaterial]);
  }
  parseRoomSwitchTargets(b, e);
  return b;
}

void XmlWorldBuilder::parseRoomSwitchTargets(Switch* s, TiXmlElement* e) {
  SwitchTargets* q = new SwitchTargets(s);
  for (TiXmlElement* t = e->FirstChildElement("target"); t != NULL;
       t = t->NextSiblingElement("target")) {
    q->addTarget(t->Attribute("name"));
  }
  switchTargets_.push_back(q);
}

void XmlWorldBuilder::resolveSwitchTargets() {
  std::vector<SwitchTargets*>::const_iterator i;
  for (i = switchTargets_.begin(); i != switchTargets_.end(); i++) {
    SwitchTargets* t = *i;
    Switch* s = t->svitch();
    std::vector<std::string>::const_iterator is;
    for (is = t->targets().begin(); is != t->targets().end(); is++) {
      s->addTarget(*transforms_[*is]);
    }
  }
  switchTargets_.clear();
}

RoomForce* XmlWorldBuilder::parseRoomConstantForce(TiXmlElement* e) {
  return new ConstantRoomForce(
      parseVector(e->FirstChildElement("min")),
      parseVector(e->FirstChildElement("max")),
      parseVector(e->FirstChildElement("force")));
}

int XmlWorldBuilder::findRoom(const char* name) {
  int i = 0;
  std::string s = name;
  std::string roomName = s.substr(0, s.find('.'));
  TiXmlElement* e = document_.FirstChildElement("world");
  for (TiXmlElement* r = e->FirstChildElement("room"); r != NULL;
       r = r->NextSiblingElement("room"), i++) {
    const char* t = r->Attribute("name");
    if ((t != NULL) && (roomName == t)) {
      return i;
    }
  }
  std::cerr << "Error: could not find room " << name << "\n";
  return -1;
}

int XmlWorldBuilder::findRoomOrigin(const char* name) {
  std::string s = name;
  int i = s.find('.');
  std::string roomName = s.substr(0, i);
  std::string originName = s.substr(i + 1);
  TiXmlElement* e = document_.FirstChildElement("world");
  for (TiXmlElement* r = e->FirstChildElement("room"); r != NULL;
       r = r->NextSiblingElement("room")) {
    const char* t = r->Attribute("name");
    if ((t != NULL) && (roomName == t)) {
      int j = 0;
      for (TiXmlElement* o = r->FirstChildElement("origin"); o != NULL;
           o = o->NextSiblingElement("origin"), j++) {
        const char* u = o->Attribute("name");
        if ((u != NULL) && (originName == u)) {
          return j;
        }
      }
      std::cerr << "Error: could not find origin " << roomName << "."
                << originName << "\n";
      return -1;
    }
  }
  std::cerr << "Error: could not find room " << roomName << "\n";
  return -1;
}

Vector XmlWorldBuilder::parseVector(TiXmlElement* e, const Vector& d) {
  return (e == NULL) ? d : parseVector(e);
}

Vector XmlWorldBuilder::parseVector(TiXmlElement* e) {
  Vector v;
  e->QueryFloatAttribute("x", &v.x);
  e->QueryFloatAttribute("y", &v.y);
  e->QueryFloatAttribute("z", &v.z);
  return v;
}

bool XmlWorldBuilder::parseBool(TiXmlElement* e, const char* name) {
  static const std::string TRUE = "true";
  return TRUE == e->Attribute(name);
}

bool XmlWorldBuilder::parseBool(TiXmlElement* e, const char* name, bool d) {
  const char* value = e->Attribute(name);
  if (value == NULL) {
    return d;
  }
  static const std::string TRUE = "true";
  return TRUE == value;
}

World* Worlds::fromFile(const char* path) {
  XmlWorldBuilder builder;
  return builder.parseWorld(path);
}
