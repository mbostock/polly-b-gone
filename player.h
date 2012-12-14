// -*- C++ -*-

#ifndef MBOSTOCK_PLAYER_H
#define MBOSTOCK_PLAYER_H

#include <vector>

#include "model.h"
#include "physics/particle.h"
#include "physics/shape.h"
#include "physics/vector.h"

namespace mbostock {

  class Player;
  class RoomObject;
  class UnaryForce;

  class PlayerModel : public Model {
  public:
    PlayerModel(const Player& player);
    virtual ~PlayerModel();

    virtual void initialize();
    virtual void display();

  private:
    float* orientation();
    void displayAxes();

    const Player& player_;
    GLUquadric* quadric_;
    float orientation_[16];
    Model* wheelModel_;
    Model* bodyModel_;

    friend class PlayerWheelModel;
    friend class PlayerBodyModel;
  };

  class Player {
  public:
    Player();

    enum Direction { NONE, LEFT, RIGHT, FORWARD, BACKWARD };

    void move(Direction d);
    void stop(Direction d);
    void stop();
    void jiggle();
    void setOrigin(const Vector& origin);
    void setVelocity(const Vector& velocity);

    void resetForces();
    void applyForce(UnaryForce& force);
    void step(const ParticleSimulator& s);
    bool intersects(const Shape& s);
    bool constrainOutside(const RoomObject& o);
    void constrainInternal();

    float mass() const;

    inline const Vector& x() const { return x_; }
    inline const Vector& y() const { return y_; }
    inline const Vector& z() const { return z_; }
    inline const Vector& origin() const { return origin_; }
    inline float leftWheelAngle() const { return leftWheel_.angle; }
    inline float rightWheelAngle() const { return rightWheel_.angle; }

    bool leftWheelFriction() const { return leftWheel_.friction(); }
    bool rightWheelFriction() const { return rightWheel_.friction(); }

    inline Model& model() { return model_; }

  private:
    class Wheel : public Particle {
    public:
      Wheel();

      inline bool friction() const { return contact; }
      void applyContact(const RoomObject& o);
      void applyForce(const Vector& z, float f);
      void applyLinearDrag(float kd);
      void applyQuadraticDrag(float kd);
      void constrainDirection(const Vector& z);

      bool contact;
      Vector contactNormal;
      Vector contactVelocity;

      float angle;
      float angleStep;
    };

    void constrainGlancing(const RoomObject& o);
    void constrainBody();

    Wheel leftWheel_;
    Wheel rightWheel_;
    Particle body_;
    Particle counterWeight_;

    Direction turnState_;
    Direction moveState_;

    Sphere sphere_;
    Vector origin_;
    Vector x_;
    Vector y_;
    Vector z_;

    PlayerModel model_;
  };

}

#endif
