// -*- C++ -*-

#ifndef MBOSTOCK_SHAPE_H
#define MBOSTOCK_SHAPE_H

#include "vector.h"

namespace mbostock {

  class Plane;
  class Sphere;

  /**
   * Represents the projection of a point onto the closest corresponding point
   * on the surface of a shape. A projection is used when the bounding sphere of
   * a particle interpenetrates a shape and, as this constitutes a collision,
   * the sphere center is projected onto the surface of the shape to determine
   * how to move the particle out in response to the collision.
   *
   * When a point is projected onto a surface, the projection includes the
   * surface normal. However, if the point is projected onto a line, the
   * corresponding normal is undefined.
   *
   * Projections define a natural order based on the length of the projection,
   * so that it is convenient to find the shortest projection given a number of
   * candidate projections.
   */
  class Projection {
  public:
    Projection();
    Projection(const Vector& x, float length);
    Projection(const Vector& x, float length, const Vector& normal);

    /** The location of the projected point on the shape's surface. */
    Vector x;

    /** The distance from the original point to the projected point. */
    float length;

    /** The surface normal at the projected point. */
    Vector normal;

    bool operator<(const Projection& p) const;
    bool operator<=(const Projection& p) const;
    bool operator>(const Projection& p) const;
    bool operator>=(const Projection& p) const;
  };

  /**
   * Represents a shape in three dimensions. Since particles are represented
   * solely as spheres, shapes need only define intersection tests with spheres,
   * and how to project points onto the closest point on the surface of the
   * shape.
   */
  class Shape {
  public:
    virtual ~Shape() {}

    /** Returns true if the specified sphere intersects with this shape. */
    virtual bool intersects(const Sphere& s) const = 0;

    /**
     * Projects the specified point onto the surface of the shape. The
     * projection specifies the closest point on the surface of the shape to the
     * specified point, the pre-computed length of the projection (the distance
     * between two points), and the surface normal at the projected point, if
     * defined.
     */
    virtual Projection project(const Vector& x) const = 0;
  };

  /** Represents a sphere using a center point and a radius. */
  class Sphere : public Shape {
  public:
    Sphere();
    Sphere(const Vector& x, float radius);

    /** The center point of the sphere. */
    inline Vector& x() { return x_; }
    inline const Vector& x() const { return x_; }

    /** The radius of the sphere. */
    inline float radius() const { return r_; }

    /** Returns true if the sphere is entirely above the specified plane. */
    bool above(const Plane& p) const;

    /** Returns true if the sphere is entirely below the specified plane. */
    bool below(const Plane& p) const;

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    Vector x_;
    float r_;
    float r2_;

    friend class AxisAlignedBox;
    friend class Cylinder;
    friend class LineSegment;
    friend class Plane;
    friend class Triangle;
  };

  /** Represents a line segment using a start point and an end point. */
  class LineSegment : public Shape {
  public:
    LineSegment();
    LineSegment(const Vector& x0, const Vector& x1);

    /** The start point of the line segment. */
    inline const Vector& x0() const { return x0_; }

    /** The end point of the line segment. */
    inline const Vector& x1() const { return x1_; }

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    Vector x0_;
    Vector x1_;
    Vector x01_;
    float l2_;

    friend class Cylinder;
  };

  /** Represents a plane using a point on the plane and a normal. */
  class Plane : public Shape {
  public:
    Plane();
    Plane(const Vector& x, const Vector& normal);

    /** A point on the plane. */
    inline const Vector& x() const { return x_; }

    /** The normal. */
    inline const Vector& normal() const { return n_; }

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    Vector x_;
    Vector n_;

    friend class Sphere;
  };

  /** Represents a triangle using three coplanar points. */
  class Triangle : public Shape {
  public:
    Triangle();
    Triangle(const Vector& x0, const Vector& x1, const Vector& x2);

    /** The first point of the triangle. */
    inline const Vector& x0() const { return x01_.x0(); }

    /** The second point of the triangle. */
    inline const Vector& x1() const { return x12_.x0(); }

    /** The third point of the triangle. */
    inline const Vector& x2() const { return x20_.x0(); }

    /** The normal of the triangle, using counterclockwise orientation. */
    inline const Vector& normal() const { return p_.normal(); }

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    bool contains(const Vector& p) const;

    LineSegment x01_;
    LineSegment x12_;
    LineSegment x20_;
    Plane p_;

    friend class Wedge;
  };

  /** Represents a quad using four coplanar points. */
  class Quad : public Shape {
  public:
    Quad();
    Quad(const Vector& x0, const Vector& x1,
         const Vector& x2, const Vector& x3);

    /** The first point of the quad. */
    inline const Vector& x0() const { return x01_.x0(); }

    /** The second point of the quad. */
    inline const Vector& x1() const { return x12_.x0(); }

    /** The third point of the quad. */
    inline const Vector& x2() const { return x23_.x0(); }

    /** The fourth point of the quad. */
    inline const Vector& x3() const { return x30_.x0(); }

    /** The normal of the quad, using counterclockwise orientation. */
    inline const Vector& normal() const { return p_.normal(); }

    /** The plane of the quad. */
    inline const Plane& plane() const { return p_; }

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    bool contains(const Vector& p) const;

    LineSegment x01_;
    LineSegment x12_;
    LineSegment x23_;
    LineSegment x30_;
    Plane p_;

    friend class Wedge;
  };

  /**
   * Represents a wedge using the four coplanar points of the top surface.
   * Points x1 and x2 define the upper edge of the wedge; points x4 and x5 lie
   * directly beneath them to define the fifth side.
   */
  class Wedge : public Shape {
  public:
    Wedge();
    Wedge(const Vector& x0, const Vector& x1,
          const Vector& x2, const Vector& x3);

    /** Returns the first (lower) point of the wedge. */
    inline const Vector& x0() const { return top_.x0(); }

    /** Returns the second (upper) point of the wedge. */
    inline const Vector& x1() const { return top_.x1(); }

    /** Returns the third (upper) point of the wedge. */
    inline const Vector& x2() const { return top_.x2(); }

    /** Returns the forth (upper) point of the wedge. */
    inline const Vector& x3() const { return top_.x3(); }

    /** Returns the fifth point of the wedge, below x1. */
    inline const Vector& x4() const { return right_.x2(); }

    /** Returns the six point of the wedge, below x2. */
    inline const Vector& x5() const { return right_.x3(); }

    /** Returns the top quad. */
    inline const Quad& top() const { return top_; }

    /** Returns the right quad. */
    inline const Quad& right() const { return right_; }

    /** Returns the bottom quad. */
    inline const Quad& bottom() const { return bottom_; }

    /** Returns the front triangle. */
    inline const Triangle& front() const { return front_; }

    /** Returns the back triangle. */
    inline const Triangle& back() const { return back_; }

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    Quad top_;
    Quad right_;
    Quad bottom_;
    Triangle front_;
    Triangle back_;
  };

  /**
   * Represents an axis-aligned bounding box (AABB) using two points. The min
   * point is the bottom back left point, and the max point is the top front
   * right point.
   */
  class AxisAlignedBox : public Shape {
  public:
    AxisAlignedBox();
    AxisAlignedBox(const Vector& min, const Vector& max);

    /** Returns the min point, x0. */
    inline const Vector& min() const { return min_; }

    /** Returns the max point, x7. */
    inline const Vector& max() const { return max_; }

    /** Returns the left bottom back point. */
    inline const Vector& x0() const { return min_; }

    /** Returns the right bottom back point. */
    inline Vector x1() const { return Vector(max_.x, min_.y, min_.z); }

    /** Returns the right bottom front point. */
    inline Vector x2() const { return Vector(max_.x, min_.y, max_.z); }

    /** Returns the left bottom front point. */
    inline Vector x3() const { return Vector(min_.x, min_.y, max_.z); }

    /** Returns the right top back point. */
    inline Vector x4() const { return Vector(max_.x, max_.y, min_.z); }

    /** Returns the left top back point. */
    inline Vector x5() const { return Vector(min_.x, max_.y, min_.z); }

    /** Returns the left top front point. */
    inline Vector x6() const { return Vector(min_.x, max_.y, max_.z); }

    /** Returns the right top front point. */
    inline const Vector& x7() const { return max_; }

    /** Returns true if this box contains the specified point. */
    bool contains(const Vector& p) const;

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    Projection projectOut(const Vector& p) const;
    Projection projectIn(const Vector& p) const;

    Vector min_;
    Vector max_;
  };

  /**
   * Represents an oriented bounding box (OBB) using eight points.
   */
  class Box : public Shape {
  public:
    Box();
    Box(const AxisAlignedBox& aab);
    Box(const Vector& c, const Vector& x, const Vector& y, const Vector& z);
    Box(const Vector& x0, const Vector& x1, const Vector& x2, const Vector& x3,
        const Vector& x4, const Vector& x5, const Vector& x6, const Vector& x7);

    /** Returns the left bottom back point. */
    inline const Vector& x0() const { return bottom_.x0(); }

    /** Returns the right bottom back point. */
    inline const Vector& x1() const { return bottom_.x1(); }

    /** Returns the right bottom front point. */
    inline const Vector& x2() const { return bottom_.x2(); }

    /** Returns the left bottom front point. */
    inline const Vector& x3() const { return bottom_.x3(); }

    /** Returns the right top back point. */
    inline const Vector& x4() const { return top_.x0(); }

    /** Returns the left top back point. */
    inline const Vector& x5() const { return top_.x1(); }

    /** Returns the left top front point. */
    inline const Vector& x6() const { return top_.x2(); }

    /** Returns the right top front point. */
    inline const Vector& x7() const { return top_.x3(); }

    /** Returns the left quad. */
    inline const Quad& left() const { return back_; }

    /** Returns the right quad. */
    inline const Quad& right() const { return right_; }

    /** Returns the bottom quad. */
    inline const Quad& bottom() const { return bottom_; }

    /** Returns the top quad. */
    inline const Quad& top() const { return top_; }

    /** Returns the back quad. */
    inline const Quad& back() const { return back_; }

    /** Returns the front quad. */
    inline const Quad& front() const { return front_; }

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    Quad top_;
    Quad bottom_;
    Quad left_;
    Quad right_;
    Quad front_;
    Quad back_;
  };

  /** Represents a cylinder as two points and a radius. */
  class Cylinder : public Shape {
  public:
    Cylinder();
    Cylinder(const Vector& x0, const Vector& x1, float radius);

    /** Returns the start point of the cylinder's main axis. */
    inline const Vector& x0() const { return axis_.x0(); }

    /** Returns the end point of the cylinder's main axis. */
    inline const Vector& x1() const { return axis_.x1(); }

    /** Returns the radius. */
    inline float radius() const { return r_; }

    /** Returns the length of the cylinder. */
    inline float length() const { return l_; }

    /** Returns the unit direction vector of the cylinder. */
    inline const Vector& z() const { return v_; }

    virtual bool intersects(const Sphere& s) const;
    virtual Projection project(const Vector& p) const;

  private:
    LineSegment axis_;
    float l_;
    float r_;
    Vector v_;
  };

};

#endif
