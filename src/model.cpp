// -*- C++ -*-

#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include "material.h"
#include "model.h"

using namespace mbostock;

WedgeModel::WedgeModel(const Wedge& wedge)
    : wedge_(wedge), material_(&Materials::blank()), topMaterial_(NULL) {
}

void WedgeModel::setMaterial(const Material& m) {
  material_ = &m;
}

void WedgeModel::setTopMaterial(const Material& m) {
  topMaterial_ = &m;
}

const Material& WedgeModel::topMaterial() const {
  return (topMaterial_ == NULL) ? *material_ : *topMaterial_;
}

void WedgeModel::display() {
  Vector size = wedge_.x2() - wedge_.x0();

  /* Top. */
  ((topMaterial_ != NULL) ? topMaterial_ : material_)->bind();
  glBegin(GL_QUADS);
  glNormalv(wedge_.top().normal());
  glTexCoord2f(0, 0);
  glVertexv(wedge_.x0());
  glTexCoord2f(sqrtf(size.x * size.x + size.y * size.y), 0);
  glVertexv(wedge_.x1());
  glTexCoord2f(sqrtf(size.x * size.x + size.y * size.y), size.z);
  glVertexv(wedge_.x2());
  glTexCoord2f(0, size.z);
  glVertexv(wedge_.x3());
  glEnd();

  material_->bind();
  glBegin(GL_QUADS);

  /* Right. */
  glNormalv(wedge_.right().normal());
  glTexCoord2f(size.z, size.y);
  glVertexv(wedge_.x2());
  glTexCoord2f(0, size.y);
  glVertexv(wedge_.x1());
  glTexCoord2f(0, 0);
  glVertexv(wedge_.x4());
  glTexCoord2f(size.z, 0);
  glVertexv(wedge_.x5());

  /* Bottom. */
  glNormalv(wedge_.bottom().normal());
  glTexCoord2f(0, 0);
  glVertexv(wedge_.x0());
  glTexCoord2f(0, size.z);
  glVertexv(wedge_.x3());
  glTexCoord2f(size.x, size.z);
  glVertexv(wedge_.x5());
  glTexCoord2f(size.x, 0);
  glVertexv(wedge_.x4());

  glEnd();
  glBegin(GL_TRIANGLES);

  /* Front. */
  glNormalv(wedge_.front().normal());
  glTexCoord2f(size.x, size.y);
  glVertexv(wedge_.x1());
  glTexCoord2f(0, 0);
  glVertexv(wedge_.x0());
  glTexCoord2f(size.x, 0);
  glVertexv(wedge_.x4());

  /* Back. */
  glNormalv(wedge_.back().normal());
  glTexCoord2f(0, 0);
  glVertexv(wedge_.x3());
  glTexCoord2f(size.x, size.y);
  glVertexv(wedge_.x2());
  glTexCoord2f(size.x, 0);
  glVertexv(wedge_.x5());

  glEnd();
}

AxisAlignedBoxModel::AxisAlignedBoxModel(const AxisAlignedBox& box)
    : box_(box), material_(&Materials::blank()), topMaterial_(NULL),
      orientation_(POSITIVE_X), u_(0.f), v_(0.f) {
}

void AxisAlignedBoxModel::setMaterial(const Material& m) {
  material_ = &m;
}

void AxisAlignedBoxModel::setTopMaterial(const Material& m) {
  topMaterial_ = &m;
}

const Material& AxisAlignedBoxModel::topMaterial() const {
  return (topMaterial_ == NULL) ? *material_ : *topMaterial_;
}

void AxisAlignedBoxModel::display() {
  Vector size = box_.x7() - box_.x0();
  Vector t4, t5, t6, t7;
  switch (orientation_) {
    case POSITIVE_X: {
      t4 = Vector(size.x + u_, 0, v_);
      t5 = Vector(u_, 0, v_);
      t6 = Vector(u_, 0, size.z + v_);
      t7 = Vector(size.x + u_, 0, size.z + v_);
      break;
    }
    case POSITIVE_Z: {
      t5 = Vector(v_, 0, u_);
      t6 = Vector(size.z + v_, 0, u_);
      t7 = Vector(size.z + v_, 0, size.x + u_);
      t4 = Vector(v_, 0, size.x + u_);
      break;
    }
    case NEGATIVE_X: {
      t4 = Vector(-u_, 0, v_);
      t5 = Vector(size.x - u_, 0, v_);
      t6 = Vector(size.x - u_, 0, size.z + v_);
      t7 = Vector(-u_, 0, size.z + v_);
      break;
    }
    case NEGATIVE_Z: {
      t5 = Vector(size.z - v_, 0, -u_);
      t6 = Vector(-v_, 0, -u_);
      t7 = Vector(-v_, 0, size.x - u_);
      t4 = Vector(size.z - v_, 0, size.x - u_);
      break;
    }
  }

  Vector y(0, size.y, 0);
  Vector t0 = t5 + y;
  Vector t1 = t4 + y;
  Vector t2 = t7 + y;
  Vector t3 = t6 + y;

  /* Top. */
  ((topMaterial_ != NULL) ? topMaterial_ : material_)->bind();
  glBegin(GL_QUADS);
  glNormal3f(0.f, 1.f, 0.f);
  glTexCoord2f(t4.x, t4.z);
  glVertexv(box_.x4());
  glTexCoord2f(t5.x, t5.z);
  glVertexv(box_.x5());
  glTexCoord2f(t6.x, t6.z);
  glVertexv(box_.x6());
  glTexCoord2f(t7.x, t7.z);
  glVertexv(box_.x7());
  glEnd();

  material_->bind();
  glBegin(GL_QUADS);

  /* Bottom. */
  glNormal3f(0.f, -1.f, 0.f);
  glTexCoord2f(t0.x, t0.z);
  glVertexv(box_.x0());
  glTexCoord2f(t1.x, t1.z);
  glVertexv(box_.x1());
  glTexCoord2f(t2.x, t2.z);
  glVertexv(box_.x2());
  glTexCoord2f(t3.x, t3.z);
  glVertexv(box_.x3());

  /* Front. */
  glNormal3f(0.f, 0.f, 1.f);
  glTexCoord2f(t7.x, t7.y);
  glVertexv(box_.x7());
  glTexCoord2f(t6.x, t6.y);
  glVertexv(box_.x6());
  glTexCoord2f(t3.x, t3.y);
  glVertexv(box_.x3());
  glTexCoord2f(t2.x, t2.y);
  glVertexv(box_.x2());

  /* Back. */
  glNormal3f(0.f, 0.f, -1.f);
  glTexCoord2f(t5.x, t5.y);
  glVertexv(box_.x5());
  glTexCoord2f(t4.x, t4.y);
  glVertexv(box_.x4());
  glTexCoord2f(t1.x, t1.y);
  glVertexv(box_.x1());
  glTexCoord2f(t0.x, t0.y);
  glVertexv(box_.x0());

  /* Left. */
  glNormal3f(-1.f, 0.f, 0.f);
  glTexCoord2f(t6.z, t6.y);
  glVertexv(box_.x6());
  glTexCoord2f(t5.z, t5.y);
  glVertexv(box_.x5());
  glTexCoord2f(t0.z, t0.y);
  glVertexv(box_.x0());
  glTexCoord2f(t3.z, t3.y);
  glVertexv(box_.x3());

  /* Right. */
  glNormal3f(1.f, 0.f, 0.f);
  glTexCoord2f(t4.z, t4.y);
  glVertexv(box_.x4());
  glTexCoord2f(t7.z, t7.y);
  glVertexv(box_.x7());
  glTexCoord2f(t2.z, t2.y);
  glVertexv(box_.x2());
  glTexCoord2f(t1.z, t1.y);
  glVertexv(box_.x1());

  glEnd();
}

void AxisAlignedBoxModel::setTexOffset(float u, float v) {
  u_ = u;
  v_ = v;
}

void AxisAlignedBoxModel::setTexOrientation(Orientation orientation) {
  orientation_ = orientation;
}

BoxModel::BoxModel(const Box& box)
    : box_(box), material_(&Materials::blank()), topMaterial_(NULL) {
}

void BoxModel::setMaterial(const Material& m) {
  material_ = &m;
}

void BoxModel::setTopMaterial(const Material& m) {
  topMaterial_ = &m;
}

const Material& BoxModel::topMaterial() const {
  return (topMaterial_ == NULL) ? *material_ : *topMaterial_;
}

void BoxModel::display() {
  Vector size(
      (box_.x1() - box_.x0()).length(),
      (box_.x5() - box_.x0()).length(),
      (box_.x3() - box_.x0()).length());

  Vector t4(size.x, 0, 0);
  Vector t5(0, 0, 0);
  Vector t6(0, 0, size.z);
  Vector t7(size.x, 0, size.z);

  Vector y(0, size.y, 0);
  Vector t0 = t5 + y;
  Vector t1 = t4 + y;
  Vector t2 = t7 + y;
  Vector t3 = t6 + y;

  /* Top. */
  ((topMaterial_ != NULL) ? topMaterial_ : material_)->bind();
  glBegin(GL_QUADS);
  glNormalv(box_.top().normal());
  glTexCoord2f(t4.x, t4.z);
  glVertexv(box_.x4());
  glTexCoord2f(t5.x, t5.z);
  glVertexv(box_.x5());
  glTexCoord2f(t6.x, t6.z);
  glVertexv(box_.x6());
  glTexCoord2f(t7.x, t7.z);
  glVertexv(box_.x7());
  glEnd();

  material_->bind();
  glBegin(GL_QUADS);

  /* Bottom. */
  glNormalv(box_.bottom().normal());
  glTexCoord2f(t0.x, t0.z);
  glVertexv(box_.x0());
  glTexCoord2f(t1.x, t1.z);
  glVertexv(box_.x1());
  glTexCoord2f(t2.x, t2.z);
  glVertexv(box_.x2());
  glTexCoord2f(t3.x, t3.z);
  glVertexv(box_.x3());

  /* Front. */
  glNormalv(box_.front().normal());
  glTexCoord2f(t7.x, t7.y);
  glVertexv(box_.x7());
  glTexCoord2f(t6.x, t6.y);
  glVertexv(box_.x6());
  glTexCoord2f(t3.x, t3.y);
  glVertexv(box_.x3());
  glTexCoord2f(t2.x, t2.y);
  glVertexv(box_.x2());

  /* Back. */
  glNormalv(box_.back().normal());
  glTexCoord2f(t5.x, t5.y);
  glVertexv(box_.x5());
  glTexCoord2f(t4.x, t4.y);
  glVertexv(box_.x4());
  glTexCoord2f(t1.x, t1.y);
  glVertexv(box_.x1());
  glTexCoord2f(t0.x, t0.y);
  glVertexv(box_.x0());

  /* Left. */
  glNormalv(box_.left().normal());
  glTexCoord2f(t6.z, t6.y);
  glVertexv(box_.x6());
  glTexCoord2f(t5.z, t5.y);
  glVertexv(box_.x5());
  glTexCoord2f(t0.z, t0.y);
  glVertexv(box_.x0());
  glTexCoord2f(t3.z, t3.y);
  glVertexv(box_.x3());

  /* Right. */
  glNormalv(box_.right().normal());
  glTexCoord2f(t4.z, t4.y);
  glVertexv(box_.x4());
  glTexCoord2f(t7.z, t7.y);
  glVertexv(box_.x7());
  glTexCoord2f(t2.z, t2.y);
  glVertexv(box_.x2());
  glTexCoord2f(t1.z, t1.y);
  glVertexv(box_.x1());

  glEnd();
}

QuadModel::QuadModel(const Quad& quad)
    : quad_(quad), material_(&Materials::blank()) {
  float x = (quad_.x1() - quad_.x0()).length();
  float y = (quad_.x3() - quad_.x0()).length();
  if (x < y) {
    texCoords_[0] = Vector(0, 0);
    texCoords_[1] = Vector(0, x);
    texCoords_[2] = Vector(y, x);
    texCoords_[3] = Vector(y, 0);
  } else {
    texCoords_[0] = Vector(0, 0);
    texCoords_[1] = Vector(x, 0);
    texCoords_[2] = Vector(x, y);
    texCoords_[3] = Vector(0, y);
  }
}

void QuadModel::setMaterial(const Material& m) {
  material_ = &m;
}

void QuadModel::setTexCoords(const Vector& t0, const Vector& t1,
                             const Vector& t2, const Vector& t3) {
  texCoords_[0] = t0;
  texCoords_[1] = t1;
  texCoords_[2] = t2;
  texCoords_[3] = t3;
}

void QuadModel::display() {
  material_->bind();
  displaySide(true);
  glFrontFace(GL_CW);
  displaySide(false);
  glFrontFace(GL_CCW);
}

void QuadModel::displaySide(bool front) {
  glBegin(GL_QUADS);
  glNormalv(front ? quad_.normal() : -quad_.normal());
  glTexCoordv(texCoords_[0]);
  glVertexv(quad_.x0());
  glTexCoordv(texCoords_[1]);
  glVertexv(quad_.x1());
  glTexCoordv(texCoords_[2]);
  glVertexv(quad_.x2());
  glTexCoordv(texCoords_[3]);
  glVertexv(quad_.x3());
  glEnd();
}

TriangleModel::TriangleModel(const Triangle& triangle)
    : triangle_(triangle), material_(&Materials::blank()) {
  float x = (triangle_.x1() - triangle_.x0()).length();
  float y = (triangle_.x2() - triangle_.x0()).length();
  texCoords_[0] = Vector(0, 0);
  texCoords_[1] = Vector(x, 0);
  texCoords_[2] = Vector(x, y);
}

void TriangleModel::setMaterial(const Material& m) {
  material_ = &m;
}

void TriangleModel::setTexCoords(const Vector& t0, const Vector& t1,
                                 const Vector& t2) {
  texCoords_[0] = t0;
  texCoords_[1] = t1;
  texCoords_[2] = t2;
}

void TriangleModel::display() {
  material_->bind();
  displaySide(true);
  glFrontFace(GL_CW);
  displaySide(false);
  glFrontFace(GL_CCW);
}

void TriangleModel::displaySide(bool front) {
  glBegin(GL_TRIANGLES);
  glNormalv(front ? triangle_.normal() : -triangle_.normal());
  glTexCoordv(texCoords_[0]);
  glVertexv(triangle_.x0());
  glTexCoordv(texCoords_[1]);
  glVertexv(triangle_.x1());
  glTexCoordv(texCoords_[2]);
  glVertexv(triangle_.x2());
  glEnd();
}

CylinderModel::CylinderModel(const Cylinder& cylinder, const Vector& y)
    : cylinder_(cylinder), y_(y),
      material_(&Materials::blank()), capMaterial_(NULL),
      quadric_(NULL) {
  for (int i = 0; i < 15; i++) {
    orientation_[i] = 0.f;
  }
  orientation_[15] = 1.f;
}

CylinderModel::~CylinderModel() {
  if (quadric_ != NULL) {
    gluDeleteQuadric(quadric_);
  }
}

void CylinderModel::initialize() {
  if (quadric_ != NULL) {
    gluDeleteQuadric(quadric_);
  }
  quadric_ = gluNewQuadric();
}

float* CylinderModel::orientation() {
  const Vector& z = cylinder_.z();
  const Vector& y = y_;
  const Vector& x = y.cross(z);
  orientation_[0] = x.x; orientation_[1] = x.y; orientation_[2] = x.z;
  orientation_[4] = y.x; orientation_[5] = y.y; orientation_[6] = y.z;
  orientation_[8] = z.x; orientation_[9] = z.y; orientation_[10] = z.z;
  return orientation_;
}

void CylinderModel::setMaterial(const Material& m) {
  material_ = &m;
}

void CylinderModel::setCapMaterial(const Material& m) {
  capMaterial_ = &m;
}

const Material& CylinderModel::capMaterial() const {
  return (capMaterial_ == NULL) ? *material_ : *capMaterial_;
}

void CylinderModel::display() {
  float r = cylinder_.radius();
  float l = cylinder_.length();
  int slices = std::max(16, (int) roundf(64 * r * r));
  int loops = 1;
  int stacks = 1;
  glPushMatrix();
  glTranslatev(cylinder_.x0());
  glMultMatrixf(orientation());
  gluQuadricTexture(quadric_, GL_TRUE);
  material_->bind();
  gluCylinder(quadric_, r, r, l, slices, stacks);
  glPushMatrix();
  glRotatef(180.f, 0.f, 1.f, 0.f);
  ((capMaterial_ != NULL) ? capMaterial_ : material_)->bind();
  gluDisk(quadric_, 0.f, r, slices, loops);
  glPopMatrix();
  glTranslatef(0.f, 0.f, l);
  gluDisk(quadric_, 0.f, r, slices, loops);
  gluQuadricTexture(quadric_, GL_FALSE);
  glPopMatrix();
}

SphereModel::SphereModel(const Sphere& sphere)
    : sphere_(sphere), material_(&Materials::blank()), quadric_(NULL) {
}

SphereModel::~SphereModel() {
  if (quadric_ != NULL) {
    gluDeleteQuadric(quadric_);
  }
}

void SphereModel::initialize() {
  if (quadric_ != NULL) {
    gluDeleteQuadric(quadric_);
  }
  quadric_ = gluNewQuadric();
}

void SphereModel::setMaterial(const Material& m) {
  material_ = &m;
}

void SphereModel::display() {
  float r2 = sphere_.radius() * sphere_.radius();
  int slices = std::max(16, (int) roundf(32 * r2));
  int stacks = std::max(4, (int) roundf(8 * r2));
  material_->bind();
  glPushMatrix();
  glTranslatev(sphere_.x());
  gluQuadricTexture(quadric_, GL_TRUE);
  gluSphere(quadric_, sphere_.radius(), slices, stacks);
  gluQuadricTexture(quadric_, GL_FALSE);
  glPopMatrix();
}

static Model* teapotModel_ = NULL;
static Model* icosahedronModel_ = NULL;

class TeapotModel : public Model {
public:
  virtual void display() {
    glFrontFace(GL_CW);
    glutSolidTeapot(1.f);
    glFrontFace(GL_CCW);
  }
};

class IcosahedronModel : public Model {
public:
  virtual void display() {
    glutSolidIcosahedron();
  }
};

class DisplayListModel : public Model {
public:
  DisplayListModel(Model* model)
      : model_(model), displayList_(GL_NONE) {
  }

  virtual ~DisplayListModel() {
    if (displayList_ != GL_NONE) {
      glDeleteLists(displayList_, 1);
    }
    delete model_;
  }

  virtual void initialize() {
    if (displayList_ != GL_NONE) {
      glDeleteLists(displayList_, 1);
    }
    displayList_ = glGenLists(1);
    glNewList(displayList_, GL_COMPILE);
    model_->initialize();
    model_->display();
    glEndList();
  }

  virtual void display() {
    glCallList(displayList_);
  }

private:
  Model* model_;
  GLuint displayList_;
};

Model* Models::compile(Model* model) {
  return new DisplayListModel(model);
}

Model* Models::teapot() {
  if (teapotModel_ == NULL) {
    teapotModel_ = new TeapotModel();
  }
  return teapotModel_;
}

Model* Models::icosahedron() {
  if (icosahedronModel_ == NULL) {
    icosahedronModel_ = new IcosahedronModel();
  }
  return icosahedronModel_;
}
