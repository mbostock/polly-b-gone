// -*- C++ -*-

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <vector>

#include "material.h"
#include "model.h"
#include "physics/constraint.h"
#include "physics/force.h"
#include "physics/particle.h"
#include "physics/shape.h"
#include "physics/vector.h"
#include "player.h"
#include "room.h"
#include "room_object.h"
#include "world.h"

using namespace mbostock;

static const float axleRadius = .02f;
static const float axleLength = .14f;
static const float bodySize = .15f;
static const float eyeRadius = .02f;
static const float wheelRadius = .1f;
static const float wheelWeight = 1.f;
static const float counterWeightRadius = .07f;
static const float counterWeightOffset = .07f;
static const float counterWeight = .1f;

static const float forwardForce = 12.f;
static const float backwardForce = 5.f;
static const float turnForce = 1.f;
static const float motorFriction = 1.f;
static const float brakeFriction = 3.f;

static const Vector spokeColor(.5f, .5f, .5f);
static const Vector eyeColor(.5f, .5f, .5f);
static const Vector axleMaterialDiffuse(.9f, .8f, .8f);
static const Vector tireMaterialDiffuse(.1f, .1f, .1f);
static const Vector bodyMaterialDiffuse(.6f, .2f, .3f);

namespace mbostock {

  /**
   * A small model which displays just the player's wheel, so that this model
   * can be compiled into a display list for faster display. This model uses the
   * parent model's quadric.
   */
  class PlayerWheelModel : public Model {
  public:
    PlayerWheelModel(const PlayerModel& model);

    virtual void display();

  private:
    const PlayerModel& model_;
  };

  /**
   * A small model which displays just the player's body (and axle, and eyes),
   * so that this model can be compiled into a display list for faster display.
   * This model uses the parent model's quadric.
   */
  class PlayerBodyModel : public Model {
  public:
    PlayerBodyModel(const PlayerModel& model);

    virtual void display();

  private:
    const PlayerModel& model_;
  };

}

PlayerWheelModel::PlayerWheelModel(const PlayerModel& model)
    : model_(model) {
}

void PlayerWheelModel::display() {
  /* Spokes. */
  glDisable(GL_LIGHTING);
  glColorv(spokeColor);
  glLineWidth(2.f);
  gluQuadricDrawStyle(model_.quadric_, GLU_LINE);
  gluDisk(model_.quadric_, 0, wheelRadius - wheelRadius / 8.f, 8, 1);
  gluQuadricDrawStyle(model_.quadric_, GLU_FILL);
  glEnable(GL_LIGHTING);

  /* Tire. */
  glMaterialv(GL_FRONT_AND_BACK, GL_DIFFUSE, tireMaterialDiffuse);
  glutSolidTorus(wheelRadius / 4.f, 3 * wheelRadius / 4.f, 16, 32);
}

PlayerBodyModel::PlayerBodyModel(const PlayerModel& model) 
  : model_(model) {
}

void PlayerBodyModel::display() {
  float l = axleLength + wheelRadius / 4.f + wheelRadius;

  /* Axle endcap. */
  glMaterialv(GL_FRONT_AND_BACK, GL_DIFFUSE, axleMaterialDiffuse);
  glPushMatrix();
  glTranslatef(0.f, 0.f, -l / 2.f);
  glRotatef(180.f, 0.f, 1.f, 0.f);
  gluDisk(model_.quadric_, 0.f, axleRadius, 16, 8);
  glPopMatrix();

  /* Axle. */
  glPushMatrix();
  glTranslatef(0.f, 0.f, -l / 2.f);
  gluCylinder(model_.quadric_, axleRadius, axleRadius, l, 16, 1);
  glPopMatrix();

  /* Axle endcap. */
  glPushMatrix();
  glTranslatef(0.f, 0.f, l / 2.f);
  gluDisk(model_.quadric_, 0.f, axleRadius, 16, 8);
  glPopMatrix();

  /* Body. */
  glMaterialv(GL_FRONT_AND_BACK, GL_DIFFUSE, bodyMaterialDiffuse);
  glutSolidCube(bodySize);

  /* Eyes. */
  static const float eyeDepth = .01f;
  glMaterialv(GL_FRONT_AND_BACK, GL_DIFFUSE, axleMaterialDiffuse);
  glColorv(eyeColor);
  glTranslatef(bodySize / 2.f, bodySize / 6.f, bodySize / 4.f);
  glPushMatrix();
  glRotatef(90.f, 0.f, 1.f, 0.f);
  gluCylinder(model_.quadric_, eyeRadius, eyeRadius, eyeDepth, 16, 1);
  glTranslatef(0.f, 0.f, eyeDepth);
  gluDisk(model_.quadric_, 0, eyeRadius, 8, 1);
  glTranslatef(0.f, -.004f, .001f);
  glMaterialv(GL_FRONT_AND_BACK, GL_DIFFUSE, tireMaterialDiffuse);
  gluDisk(model_.quadric_, 0, eyeRadius / 2.f, 8, 1);
  glMaterialv(GL_FRONT_AND_BACK, GL_DIFFUSE, axleMaterialDiffuse);
  glPopMatrix();
  glTranslatef(0.f, 0.f, -bodySize / 2.f);
  glPushMatrix();
  glRotatef(90.f, 0.f, 1.f, 0.f);
  gluCylinder(model_.quadric_, eyeRadius, eyeRadius, eyeDepth, 16, 1);
  glTranslatef(0.f, 0.f, eyeDepth);
  gluDisk(model_.quadric_, 0, eyeRadius, 8, 1);
  glTranslatef(0.f, -.004f, .001f);
  glMaterialv(GL_FRONT_AND_BACK, GL_DIFFUSE, tireMaterialDiffuse);
  gluDisk(model_.quadric_, 0, eyeRadius / 2.f, 8, 1);
  glPopMatrix();
}

PlayerModel::PlayerModel(const Player& player)
  : player_(player), quadric_(NULL),
    wheelModel_(Models::compile(new PlayerWheelModel(*this))),
    bodyModel_(Models::compile(new PlayerBodyModel(*this))) {
  for (int i = 0; i < 15; i++) {
    orientation_[i] = 0.f;
  }
  orientation_[15] = 1.f;
}

PlayerModel::~PlayerModel() {
  delete wheelModel_;
  delete bodyModel_;
  if (quadric_ != NULL) {
    gluDeleteQuadric(quadric_);
  }
}

void PlayerModel::initialize() {
  if (quadric_ != NULL) {
    gluDeleteQuadric(quadric_);
  }
  quadric_ = gluNewQuadric();
  wheelModel_->initialize();
  bodyModel_->initialize();
}

float* PlayerModel::orientation() {
  const Vector& x = player_.x();
  const Vector& y = player_.y();
  const Vector& z = player_.z();
  orientation_[0] = x.x; orientation_[1] = x.y; orientation_[2] = x.z;
  orientation_[4] = y.x; orientation_[5] = y.y; orientation_[6] = y.z;
  orientation_[8] = z.x; orientation_[9] = z.y; orientation_[10] = z.z;
  return orientation_;
}

void PlayerModel::display() {
  Materials::blank().bind();

  glPushMatrix();
  glTranslatev(player_.origin());
  glMultMatrixf(orientation());

  /* Left wheel. */
  if (!World::world()->debug() || player_.leftWheelFriction()) {
    glPushMatrix();
    glTranslatef(0.f, 0.f, -axleLength / 2.f - wheelRadius / 2.f);
    glRotatef(player_.leftWheelAngle(), 0.f, 0.f, 1.f);
    glRotatef(180.f, 0.f, 1.f, 0.f);
    wheelModel_->display();
    glPopMatrix();
  }

  /* Right wheel. */
  if (!World::world()->debug() || player_.rightWheelFriction()) {
    glPushMatrix();
    glTranslatef(0.f, 0.f, axleLength / 2.f + wheelRadius / 2.f);
    glRotatef(player_.rightWheelAngle(), 0.f, 0.f, 1.f);
    wheelModel_->display();
    glPopMatrix();
  }

  /* Axes. */
  if (World::world()->debug()) {
    displayAxes();
  }

  /* Body. */
  bodyModel_->display();
  glPopMatrix();
}

void PlayerModel::displayAxes() {
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3f(1.f, 0.f, 0.f);
  glVertex3f(0.f, 0.f, 0.f);
  glVertex3f(2 * bodySize, 0.f, 0.f);
  glColor3f(0.f, 1.f, 0.f);
  glVertex3f(0.f, 0.f, 0.f);
  glVertex3f(0.f, 2 * bodySize, 0.f);
  glColor3f(0.f, 0.f, 1.f);
  glVertex3f(0.f, 0.f, 0.f);
  glVertex3f(0.f, 0.f, 2 * bodySize);
  glEnd();
  glEnable(GL_LIGHTING);
}

Player::Player()
    : turnState_(NONE), moveState_(NONE),
      sphere_(Vector::ZERO(), wheelRadius * 2.f), model_(*this) {
  counterWeight_.inverseMass = 1.f / counterWeight;
}

float Player::mass() const {
  return counterWeight + 3.f;
}

Player::Wheel::Wheel()
    : angle(0.f), angleStep(0.f) {
  inverseMass = 1.f / wheelWeight;
}

void Player::setOrigin(const Vector& origin) {
  static const float a2 = axleLength / 2.f;

  leftWheel_.position = origin + Vector(0.f, 0.f, -a2);
  rightWheel_.position = origin + Vector(0.f, 0.f, a2);
  body_.position = origin;
  counterWeight_.position = origin + Vector(-counterWeightOffset, 0.f, 0.f);

  leftWheel_.previousPosition = leftWheel_.position;
  rightWheel_.previousPosition = rightWheel_.position;
  body_.previousPosition = body_.position;
  counterWeight_.previousPosition = counterWeight_.position;

  origin_ = origin;
  x_ = Vector::X();
  y_ = Vector::Y();
  z_ = Vector::Z();
}

void Player::setVelocity(const Vector& velocity) {

  /* TODO: Rotate the player to face the velocity. But this works for now. */
  if (velocity.x < 0) {
    static const float a2 = axleLength / 2.f;

    leftWheel_.position = origin_ + Vector(0.f, 0.f, a2);
    rightWheel_.position = origin_ + Vector(0.f, 0.f, -a2);
    counterWeight_.position = origin_ + Vector(counterWeightOffset, 0.f, 0.f);

    leftWheel_.previousPosition = leftWheel_.position;
    rightWheel_.previousPosition = rightWheel_.position;
    counterWeight_.previousPosition = counterWeight_.position;
  }

  Vector v = velocity * ParticleSimulator::timeStep();
  leftWheel_.previousPosition = leftWheel_.position - v;
  rightWheel_.previousPosition = rightWheel_.position - v;
  body_.previousPosition = body_.position - v;
  counterWeight_.previousPosition = counterWeight_.position - v;
}

void Player::resetForces() {
  leftWheel_.force = Vector::ZERO();
  rightWheel_.force = Vector::ZERO();
  body_.force = Vector::ZERO();
  counterWeight_.force = Vector::ZERO();

  /* Moving forces. */
  switch (moveState_) {
    case FORWARD: {
      leftWheel_.applyForce(z_, forwardForce);
      rightWheel_.applyForce(z_, forwardForce);
      break;
    }
    case BACKWARD: {
      leftWheel_.applyForce(z_, -backwardForce);
      rightWheel_.applyForce(z_, -backwardForce);
      break;
    }
  }

  /* Turning forces. */
  switch (turnState_) {
    case RIGHT: {
      leftWheel_.applyForce(z_, turnForce);
      rightWheel_.applyForce(z_, -turnForce);
      break;
    }
    case LEFT: {
      leftWheel_.applyForce(z_, -turnForce);
      rightWheel_.applyForce(z_, turnForce);
      break;
    }
  }

  /* Uniform drag forces. */
  if ((turnState_ != NONE) || (moveState_ == FORWARD)) {
    leftWheel_.applyQuadraticDrag(motorFriction);
    rightWheel_.applyQuadraticDrag(motorFriction);
  } else {
    leftWheel_.applyLinearDrag(brakeFriction);
    rightWheel_.applyLinearDrag(brakeFriction);
  }

  /* Flipping force if upside-down. */
  if ((leftWheel_.contact && rightWheel_.contact)
      && (y_.y < (.5 * leftWheel_.contactNormal.y))) {
    counterWeight_.force -= y_;
  }
}

void Player::Wheel::applyContact(const RoomObject& o) {
  const Vector& normal = Constraints::projection().normal;
  if ((normal.y > o.slip()) && (!contact || (contactNormal.y < normal.y))) {
    contactNormal = normal;
    contactVelocity = o.velocity(position);
    contact = true;
  }
}

void Player::Wheel::applyForce(const Vector& z, float f) {
  if (friction()) {
    force += z.cross(contactNormal) * -f;
  }
}

void Player::Wheel::applyLinearDrag(float kd) {
  if (friction()) {
    Vector v = (position - previousPosition - contactVelocity)
        / ParticleSimulator::timeStep();
    float vs = v.squared();
    if (vs < 1E-3) {
      kd *= 10.f;
    } else if (vs < 1E-2) {
      kd *= 2.f;
    }
    force -= v * kd;
  }
}

void Player::Wheel::applyQuadraticDrag(float kd) {
  if (friction()) {
    Vector v = (position - previousPosition - contactVelocity)
        / ParticleSimulator::timeStep();
    force -= v * v.length() * kd;
  }
}

void Player::applyForce(UnaryForce& force) {
  force.apply(leftWheel_);
  force.apply(rightWheel_);
  force.apply(body_);
  force.apply(counterWeight_);
}

void Player::step(const ParticleSimulator& s) {
  leftWheel_.contact = false;
  rightWheel_.contact = false;
  s.step(leftWheel_);
  s.step(rightWheel_);
  s.step(body_);
  s.step(counterWeight_);
  sphere_.x() = body_.position;
}

bool Player::intersects(const Shape& s) {
  return s.intersects(sphere_);
}

bool Player::constrainOutside(const RoomObject& o) {
  const Shape& s = o.shape();
  bool contact = false;
  if (s.intersects(sphere_)) {
    if (Constraints::outside(leftWheel_, s, wheelRadius)) {
      constrainGlancing(o);
      leftWheel_.applyContact(o);
      contact = true;
    }
    if (Constraints::outside(rightWheel_, s, wheelRadius)) { 
      constrainGlancing(o);
      rightWheel_.applyContact(o);
      contact = true;
    }
    if (Constraints::outside(body_, s, wheelRadius)) {
      sphere_.x() = body_.position;
      contact = true;
    }
    if (Constraints::outside(counterWeight_, s, counterWeightRadius)) {
      contact = true;
    }
  }
  return contact;
}

void Player::constrainGlancing(const RoomObject& o) {
  const Projection& j = Constraints::projection();

  /*
   * This constraint only applies if we are moving parallel to the wall, so if
   * the body particle isn't moving, don't do anything.
   */
  if ((body_.position - body_.previousPosition).squared() < 1E-5) {
    return;
  }

  /* For glancing collisions, align parallel to the wall. */
  float n = j.normal.dot(z_);
  if (fabsf(n) > .8f) {
    Plane p = Plane(j.x, j.normal);
    Vector x = p.project((leftWheel_.position + rightWheel_.position) / 2.f).x;

    Vector wp1 = x + j.normal * wheelRadius;
    Vector wp2 = x + j.normal * (wheelRadius + axleLength);
    Vector bp = x + j.normal * (wheelRadius + axleLength / 2.f);
    const Vector& lp = (n > 0.f) ? wp1 : wp2;
    const Vector& rp = (n > 0.f) ? wp2 : wp1;

    /*
     * Only allow this constraint to move the player a small amount, as we don't
     * want this constraint to apply in weird cases like hitting a corner.
     * Otherwise, this constraint might throw the player large distances.
     */
    if (((bp - body_.position).squared()
           + (lp - leftWheel_.position).squared()
           + (rp - rightWheel_.position).squared()) > 1E-3) {
      return;
    }

    body_.position = bp;
    leftWheel_.position = lp;
    rightWheel_.position = rp;

    /*
     * Include a small fudge factor to push the player away from the wall, so
     * that the player can turn. Otherwise, the player seemingly gets locked to
     * the wall surface!
     */
    body_.position += j.normal * 1E-4;
    leftWheel_.position += j.normal * 1E-4;
    rightWheel_.position += j.normal * 1E-4;
    body_.previousPosition += j.normal * 1E-4;
    leftWheel_.previousPosition += j.normal * 1E-4;
    rightWheel_.previousPosition += j.normal * 1E-4;
  }
}

void Player::constrainBody() {
  Constraints::distance(leftWheel_, rightWheel_, axleLength);
  Constraints::distance(leftWheel_, body_, axleLength / 2.f);
  Constraints::distance(rightWheel_, body_, axleLength / 2.f);

  static const float d
      = sqrtf(axleLength * axleLength / 4.f
          + counterWeightOffset * counterWeightOffset);
  Constraints::distance(leftWheel_, counterWeight_, d);
  Constraints::distance(rightWheel_, counterWeight_, d);
  Constraints::distance(body_, counterWeight_, counterWeightOffset);
}

void Player::constrainInternal() {
  leftWheel_.constrainDirection(z_);
  rightWheel_.constrainDirection(z_);
  constrainBody();

  origin_ = body_.position;
  z_ = (rightWheel_.position - leftWheel_.position) / axleLength;
  x_ = (body_.position - counterWeight_.position) / counterWeightOffset;
  y_ = -x_.cross(z_);
}

void Player::Wheel::constrainDirection(const Vector& z) {
  if (!friction()) {
    angle += angleStep;
    return; // no normal force, no friction
  }

  /* Don't let upwards-moving platforms launch us into the air. */
  Vector cv = contactVelocity;
  if (cv.y > 0) {
    cv.y = 0.f;
  }

  /* Cancel the sideways component of the velocity. */
  Vector v = position - previousPosition - cv;
  v -= z * z.dot(v);
  previousPosition = position - v - cv;

  /* Rotate the wheel. */
  Vector forward = z.cross(contactNormal);
  Vector vForward = forward * forward.dot(v);
  float l = vForward.length();
  if (vForward.cross(contactNormal).dot(z) > 0) {
    l *= -1;
  }
  angleStep = 360.f * l / (M_PI * 2 * wheelRadius);
  angle += angleStep;
}

void Player::jiggle() {
  Vector v = Vector::randomVector(ParticleSimulator::timeStep());
  v.y = ParticleSimulator::timeStep();
  leftWheel_.position += v;
  rightWheel_.position += v;
  body_.position += v;
  counterWeight_.position += v;
}

void Player::move(Direction d) {
  switch (d) {
    case LEFT: {
      if (turnState_ == NONE) {
        turnState_ = LEFT;
      } else {
        turnState_ = NONE;
      }
      break;
    }
    case RIGHT: {
      if (turnState_ == NONE) {
        turnState_ = RIGHT;
      } else {
        turnState_ = NONE;
      }
      break;
    }
    case FORWARD: {
      if (moveState_ == NONE) {
        moveState_ = FORWARD;
      } else {
        moveState_ = NONE;
      }
      break;
    }
    case BACKWARD: {
      if (moveState_ == NONE) {
        moveState_ = BACKWARD;
      } else {
        moveState_ = NONE;
      }
      break;
    }
  }
}

void Player::stop() {
  turnState_ = NONE;
  moveState_ = NONE;
}

void Player::stop(Direction d) {
  switch (d) {
    case LEFT: {
      if (turnState_ != NONE) {
        turnState_ = NONE;
      } else {
        turnState_ = RIGHT;
      }
      break;
    }
    case RIGHT: {
      if (turnState_ != NONE) {
        turnState_ = NONE;
      } else {
        turnState_ = LEFT;
      }
      break;
    }
    case FORWARD: {
      if (moveState_ != NONE) {
        moveState_ = NONE;
      } else {
        moveState_ = BACKWARD;
      }
      break;
    }
    case BACKWARD: {
      if (moveState_ != NONE) {
        moveState_ = NONE;
      } else {
        moveState_ = FORWARD;
      }
      break;
    }
  }
}
