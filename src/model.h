// -*- C++ -*-

#ifndef MBOSTOCK_MODEL_H
#define MBOSTOCK_MODEL_H

#include <OpenGL/glu.h>

#include "physics/shape.h"
#include "physics/vector.h"

namespace mbostock {

  class Material;

  /**
   * A generic base class for OpenGL models. Provides a set of convenience
   * methods based on the Vector class. The constructor should not assume that
   * OpenGL is initialized; instead, use the initialize method. Note that a
   * model may be initialized multiple times if the OpenGL context is destroyed
   * and recreated, as is the case with resizing the window or toggling
   * fullscreen mode.
   */
  class Model {
  public:
    virtual ~Model() {}

    /** Initializes any OpenGL state used by the model. */
    virtual void initialize() {}

    /** Displays the model. */
    virtual void display() = 0;

  protected:

    /** A convenience wrapper for glTranslatef. */
    inline void glTranslatev(const Vector& v) const {
      glTranslatef(v.x, v.y, v.z);
    }

    /** A convenience wrapper for glRotatef. */
    inline void glRotatev(float angle, const Vector& axis) const {
      glRotatef(angle, axis.x, axis.y, axis.z);
    }

    /** A convenience wrapper for glVertex3f. */
    inline void glVertexv(const Vector& v) const {
      glVertex3f(v.x, v.y, v.z);
    }

    /** A convenience wrapper for glTexCoord2f. Uses the x and y coordinate. */
    inline void glTexCoordv(const Vector& v) const {
      glTexCoord2f(v.x, v.y);
    }

    /** A convenience wrapper for glNormal3f. */
    inline void glNormalv(const Vector& v) const {
      glNormal3f(v.x, v.y, v.z);
    }

    /** A convenience wrapper for glMaterialfv. */
    inline void glMaterialv(GLenum face, GLenum pname, const Vector& v) const {
      GLfloat params[] = { v.x, v.y, v.z, 1.f };
      glMaterialfv(face, pname, params);
    }

    /** A convenience wrapper for glColor3f. */
    inline void glColorv(const Vector& v) const {
      glColor3f(v.x, v.y, v.z);
    }
  };

  /**
   * A model for Wedge. A separate material may be specified for the top face of
   * the wedge.
   */
  class WedgeModel : public Model {
  public:
    WedgeModel(const Wedge& wedge);

    virtual void display();

    void setMaterial(const Material& m);
    void setTopMaterial(const Material& m);

    inline const Material& material() const { return *material_; }
    const Material& topMaterial() const;

  private:
    const Wedge& wedge_;
    const Material* material_;
    const Material* topMaterial_;
  };

  /**
   * A model for AxisAlignedBox. A separate material may be specified for the
   * top face of the box. In addition, the texture may be oriented along either
   * the X or Z axis, and the texture may be offset (e.g., for escalatars).
   */
  class AxisAlignedBoxModel : public Model {
  public:
    AxisAlignedBoxModel(const AxisAlignedBox& box);

    virtual void display();

    enum Orientation { POSITIVE_X, NEGATIVE_X, POSITIVE_Z, NEGATIVE_Z };

    void setMaterial(const Material& m);
    void setTopMaterial(const Material& m);
    void setTexOffset(float u, float v);
    void setTexOrientation(Orientation orientation);

    inline const Material& material() const { return *material_; }
    const Material& topMaterial() const;

  private:
    const AxisAlignedBox& box_;
    const Material* material_;
    const Material* topMaterial_;
    Orientation orientation_;
    float u_, v_;
  };

  /**
   * A model for Box. A separate material may be specified for the top face of
   * the box.
   */
  class BoxModel : public Model {
  public:
    BoxModel(const Box& box);

    virtual void display();

    void setMaterial(const Material& m);
    void setTopMaterial(const Material& m);

    inline const Material& material() const { return *material_; }
    const Material& topMaterial() const;

  private:
    const Box& box_;
    const Material* material_;
    const Material* topMaterial_;
  };

  /**
   * A model for Quad. The texture coordinates may be specified explicitly, or
   * they will be inferred from the vertex coordinates.
   */
  class QuadModel : public Model {
  public:
    QuadModel(const Quad& quad);

    virtual void display();

    void setMaterial(const Material& m);
    void setTexCoords(const Vector& t0, const Vector& t1,
                      const Vector& t2, const Vector& t3);

    inline const Material& material() const { return *material_; }

  private:
    void displaySide(bool front);

    const Quad& quad_;
    const Material* material_;
    Vector texCoords_[4];
  };

  /**
   * A model for Triangle. The texture coordinates may be specified explicitly,
   * or they will be inferred from the vertex coordinates.
   */
  class TriangleModel : public Model {
  public:
    TriangleModel(const Triangle& triangle);

    virtual void display();

    void setMaterial(const Material& m);
    void setTexCoords(const Vector& t0, const Vector& t1, const Vector& t2);

    inline const Material& material() const { return *material_; }

  private:
    void displaySide(bool front);

    const Triangle& triangle_;
    const Material* material_;
    Vector texCoords_[3];
  };

  /**
   * A cylinder model. A separate material may be specified for the end caps. An
   * additional Y-axis is required to orient the texture on the cylinder's
   * surface; this axis must be orthogonal to the main axis of the cylinder. A
   * reference to the Y-axis vector is retained so that changes to the Y-axis
   * will be propagated to the displayed model.
   */
  class CylinderModel : public Model {
  public:
    CylinderModel(const Cylinder& cylinder, const Vector& y);
    virtual ~CylinderModel();

    virtual void initialize();
    virtual void display();

    void setMaterial(const Material& m);
    void setCapMaterial(const Material& m);

    inline const Material& material() const { return *material_; }
    const Material& capMaterial() const;

  private:
    float* orientation();

    const Cylinder& cylinder_;
    const Vector& y_;
    const Material* material_;
    const Material* capMaterial_;
    GLUquadric* quadric_;
    float orientation_[16];
  };

  /** A model for Sphere. */
  class SphereModel : public Model {
  public:
    SphereModel(const Sphere& sphere);
    virtual ~SphereModel();

    virtual void initialize();
    virtual void display();

    void setMaterial(const Material& m);
    inline const Material& material() const { return *material_; }

  private:
    float* orientation();

    const Sphere& sphere_;
    const Material* material_;
    GLUquadric* quadric_;
    float orientation_[16];
  };

  class Models {
  public:
    static Model* teapot();
    static Model* icosahedron();
    static Model* compile(Model* model);

  private:
    Models();
  };

}

#endif
