// -*- C++ -*-

#include <algorithm>
#include <math.h>

#include "shape.h"

using namespace mbostock;

Projection::Projection() : length(0) {
}

Projection::Projection(const Vector& x, float length)
    : x(x), length(length) {
}

Projection::Projection(const Vector& x, float length, const Vector& normal)
    : x(x), length(length), normal(normal) {
}

bool Projection::operator<(const Projection& p) const {
  return fabsf(length) < fabsf(p.length);
}

bool Projection::operator<=(const Projection& p) const {
  return fabsf(length) <= fabsf(p.length);
}

bool Projection::operator>(const Projection& p) const {
  return fabsf(length) > fabsf(p.length);
}

bool Projection::operator>=(const Projection& p) const {
  return fabsf(length) >= fabsf(p.length);
}

Sphere::Sphere()
    : r_(0.f), r2_(0.f) {
}

Sphere::Sphere(const Vector& x, float radius)
    : x_(x), r_(radius), r2_(radius * radius) {
}

bool Sphere::above(const Plane& p) const {
  /* Derived from Point-Plane Distance on MathWorld. */
  return p.n_.dot(x_ - p.x_) > r_;
}

bool Sphere::below(const Plane& p) const {
  /* Derived from Point-Plane Distance on MathWorld. */
  return p.n_.dot(x_ - p.x_) < -r_;
}

bool Sphere::intersects(const Sphere& s) const {
  /* Derived from Moller-Haines section 16.13.1. */
  float r = r_ + s.r_;
  return (s.x_ - x_).squared() < (r * r);
}

Projection Sphere::project(const Vector& x) const {
  Vector v = x - x_;
  float l = v.length();
  float d = l - r_;
  Vector n = v / l;
  return Projection(x - n * d, fabsf(d), (d < 0.f) ? -n : n);
}

LineSegment::LineSegment() {
}

LineSegment::LineSegment(const Vector& x0, const Vector& x1)
    : x0_(x0), x1_(x1), x01_(x1 - x0), l2_(x01_.dot(x01_)) {
}

bool LineSegment::intersects(const Sphere& s) const {
  /* Derived from Point-Line Distance--3-Dimensional on MathWorld. */
  return (x01_.cross(x0_ - s.x_).squared() / l2_) < s.r2_;
}

Projection LineSegment::project(const Vector& p) const {
  /* Derived from Point-Line Distance--3-Dimensional on MathWorld. */
  float t = (p - x0_).dot(x01_) / l2_;
  Vector x = (t <= 0.f) ? x0_ : ((t >= 1.f) ? x1_ : (x0_ + x01_ * t));
  return Projection(x, (p - x).length());
}

Plane::Plane() {
}

Plane::Plane(const Vector& x, const Vector& normal)
    : x_(x), n_(normal) {
}

bool Plane::intersects(const Sphere& s) const {
  /* Derived from Point-Plane Distance on MathWorld. */
  return fabsf(n_.dot(s.x_ - x_)) < s.r_;
}

Projection Plane::project(const Vector& p) const {
  /* Derived from Point-Plane Distance on MathWorld. */
  float l = n_.dot(p - x_);
  return Projection(p - n_ * l, fabsf(l), (l < 0.f) ? -n_ : n_);
}

Triangle::Triangle() {
}

Triangle::Triangle(const Vector& x0, const Vector& x1, const Vector& x2)
    : x01_(x0, x1), x12_(x1, x2), x20_(x2, x0),
      p_(x0, (x1 - x0).cross(x2 - x1).normalize()) {
}

bool Triangle::intersects(const Sphere& s) const {
  /* Derived from ERIT section 2.3 (reordered). */
  Projection p0 = p_.project(s.x_);
  if (p0.length > s.r_) {
    return false;
  }
  if (x01_.intersects(s) || x12_.intersects(s) || x20_.intersects(s)) {
    return true;
  }
  return contains(p0.x);
}

Projection Triangle::project(const Vector& p) const {
  Projection pp = p_.project(p);
  if (contains(pp.x)) {
    return pp;
  }
  Projection p01 = x01_.project(pp.x);
  Projection p12 = x12_.project(pp.x);
  Projection p20 = x20_.project(pp.x);
  Projection pb = std::min(std::min(p01, p12), p20);
  pb.length = sqrtf(pb.length * pb.length + pp.length * pp.length);
  pb.normal = pp.normal;
  return pb;
}

bool Triangle::contains(const Vector& p) const {
  return ((x0() - x1()).cross(p - x0()).dot(p_.normal()) < 0.f)
      && ((x1() - x2()).cross(p - x1()).dot(p_.normal()) < 0.f)
      && ((x2() - x0()).cross(p - x2()).dot(p_.normal()) < 0.f);
}

Quad::Quad() {
}

Quad::Quad(const Vector& x0, const Vector& x1,
           const Vector& x2, const Vector& x3)
    : x01_(x0, x1), x12_(x1, x2), x23_(x2, x3), x30_(x3, x0),
      p_(x0, (x1 - x0).cross(x2 - x1).normalize()) {
}

bool Quad::intersects(const Sphere& s) const {
  /* Derived from Triangle-Sphere test above. */
  Projection p0 = p_.project(s.x());
  if (p0.length > s.radius()) {
    return false;
  }
  if (x01_.intersects(s)
      || x12_.intersects(s)
      || x23_.intersects(s)
      || x30_.intersects(s)) {
    return true;
  }
  return contains(p0.x);
}

Projection Quad::project(const Vector& p) const {
  Projection pp = p_.project(p);
  if (contains(pp.x)) {
    return pp;
  }
  Projection p01 = x01_.project(pp.x);
  Projection p12 = x12_.project(pp.x);
  Projection p23 = x23_.project(pp.x);
  Projection p30 = x30_.project(pp.x);
  Projection pb = std::min(std::min(std::min(p01, p12), p23), p30);
  pb.length = sqrtf(pb.length * pb.length + pp.length * pp.length);
  pb.normal = pp.normal;
  return pb;
}

bool Quad::contains(const Vector& p) const {
  return ((x0() - x1()).cross(p - x0()).dot(p_.normal()) < 0.f)
      && ((x1() - x2()).cross(p - x1()).dot(p_.normal()) < 0.f)
      && ((x2() - x3()).cross(p - x2()).dot(p_.normal()) < 0.f)
      && ((x3() - x0()).cross(p - x3()).dot(p_.normal()) < 0.f);
}

Wedge::Wedge() {
}

Wedge::Wedge(const Vector& x0, const Vector& x1,
             const Vector& x2, const Vector& x3)
    : top_(x0, x1, x2, x3),
      right_(x2, x1, Vector(x1.x, x0.y, x1.z), Vector(x2.x, x0.y, x2.z)),
      bottom_(x0, x3, right_.x3(), right_.x2()),
      front_(x1, x0, right_.x2()),
      back_(x3, x2, right_.x3()) {
}

bool Wedge::intersects(const Sphere& s) const {
  /* Derived (very approximately) from Moller-Haines section 16.14.2. */
  if (s.above(top_.p_)
      || s.above(right_.p_)
      || s.above(front_.p_)
      || s.above(back_.p_)
      || s.above(bottom_.p_)) {
    return false;
  }
  if (s.below(top_.p_)
      && s.below(right_.p_)
      && s.below(front_.p_)
      && s.below(back_.p_)
      && s.below(bottom_.p_)) {
    return true;
  }
  return top_.intersects(s)
    || right_.intersects(s)
    || front_.intersects(s)
    || back_.intersects(s)
    || bottom_.intersects(s);
}

Projection Wedge::project(const Vector& p) const {
  Projection pt = top_.project(p);
  Projection pr = right_.project(p);
  Projection pf = front_.project(p);
  Projection pb = back_.project(p);
  Projection pd = bottom_.project(p);
  return std::min(std::min(std::min(std::min(pt, pr), pf), pb), pd);
}

AxisAlignedBox::AxisAlignedBox() {
}

AxisAlignedBox::AxisAlignedBox(const Vector& min, const Vector& max)
    : min_(min), max_(max) {
}

bool AxisAlignedBox::intersects(const Sphere& s) const {
  /* Derived from Moller-Haines section 16.13.2. */
  Vector e = Vector::max(min_ - s.x_, Vector::ZERO())
           + Vector::max(s.x_ - max_, Vector::ZERO());
  return e.squared() < s.r2_;
}

bool AxisAlignedBox::contains(const Vector& p) const {
  return ((p.x >= min_.x) && (p.x < max_.x)
          && (p.y >= min_.y) && (p.y < max_.y)
          && (p.z >= min_.z) && (p.z < max_.z));
}

Projection AxisAlignedBox::project(const Vector& p) const {
  return contains(p) ? projectOut(p) : projectIn(p);
}

Projection AxisAlignedBox::projectOut(const Vector& p) const {
  Vector x = p;
  Vector n;
  float l;

  float dx1 = p.x - min_.x;
  float dx2 = max_.x - p.x;
  float dx = std::min(dx1, dx2);

  float dy1 = p.y - min_.y;
  float dy2 = max_.y - p.y;
  float dy = std::min(dy1, dy2);

  float dz1 = p.z - min_.z;
  float dz2 = max_.z - p.z;
  float dz = std::min(dz1, dz2);

  if ((dx <= dy) && (dx <= dz)) {
    if (dx1 <= dx2) {
      l = dx1;
      x.x = min_.x;
      n.x = 1.f;
    } else {
      l = dx2;
      x.x = max_.x;
      n.x = -1.f;
    }
  } else if (dy <= dz) {
    if (dy1 <= dy2) {
      l = dy1;
      x.y = min_.y;
      n.y = 1.f;
    } else {
      l = dy2;
      x.y = max_.y;
      n.y = -1.f;
    }
  } else {
    if (dz1 <= dz2) {
      l = dz1;
      x.z = min_.z;
      n.z = 1.f;
    } else {
      l = dz2;
      x.z = max_.z;
      n.z = -1.f;
    }
  }

  return Projection(x, -l, n);
}

Projection AxisAlignedBox::projectIn(const Vector& p) const {
  Vector x = p;
  Vector n;

  if (p.x < min_.x) {
    x.x = min_.x;
    n = -Vector::X();
  } else if (p.x >= max_.x) {
    x.x = max_.x;
    n = Vector::X();
  }

  if (p.z < min_.z) {
    x.z = min_.z;
    n = -Vector::Z();
  } else if (p.z >= max_.z) {
    x.z = max_.z;
    n = Vector::Z();
  }

  if (p.y < min_.y) {
    x.y = min_.y;
    n = -Vector::Y();
  } else if (p.y >= max_.y) {
    x.y = max_.y;
    n = Vector::Y();
  }

  /*
   * TODO: If the particle does not project directly onto the triangle (and
   * similarly for quads), we should interpolate the normal of the projection so
   * that particles appear to bounce off the corner.
   *
   * On the other hand, this leads to some weird behavior when applying
   * directional friction when Polly is on the edge of a platform. So for now,
   * just use axis normals.
   */

  return Projection(x, (p - x).length(), n);
}

Box::Box() {
}

Box::Box(const AxisAlignedBox& box)
    : top_(box.x4(), box.x5(), box.x6(), box.x7()),
      bottom_(box.x0(), box.x1(), box.x2(), box.x3()),
      left_(box.x0(), box.x3(), box.x6(), box.x5()),
      right_(box.x1(), box.x4(), box.x7(), box.x2()),
      front_(box.x2(), box.x7(), box.x6(), box.x3()),
      back_(box.x0(), box.x5(), box.x4(), box.x1()) {
}

Box::Box(const Vector& c, const Vector& x, const Vector& y, const Vector& z) {
  Vector x0 = c - x - y - z;
  Vector x1 = c + x - y - z;
  Vector x2 = c + x - y + z;
  Vector x3 = c - x - y + z;
  Vector x4 = c + x + y - z;
  Vector x5 = c - x + y - z;
  Vector x6 = c - x + y + z;
  Vector x7 = c + x + y + z;
  top_ = Quad(x4, x5, x6, x7);
  bottom_ = Quad(x0, x1, x2, x3);
  left_ = Quad(x0, x3, x6, x5);
  right_ = Quad(x1, x4, x7, x2);
  front_ = Quad(x2, x7, x6, x3);
  back_ = Quad(x0, x5, x4, x1);
}

Box::Box(const Vector& x0, const Vector& x1, const Vector& x2, const Vector& x3,
         const Vector& x4, const Vector& x5, const Vector& x6, const Vector& x7)
    : top_(x4, x5, x6, x7),
      bottom_(x0, x1, x2, x3),
      left_(x0, x3, x6, x5),
      right_(x1, x4, x7, x2),
      front_(x2, x7, x6, x3),
      back_(x0, x5, x4, x1) {
}

bool Box::intersects(const Sphere& s) const {
  /* Derived (very approximately) from Moller-Haines section 16.14.2. */
  if (s.above(top_.plane())
      || s.above(left_.plane())
      || s.above(right_.plane())
      || s.above(front_.plane())
      || s.above(back_.plane())
      || s.above(bottom_.plane())) {
    return false;
  }
  if (s.below(top_.plane())
      && s.below(left_.plane())
      && s.below(right_.plane())
      && s.below(front_.plane())
      && s.below(back_.plane())
      && s.below(bottom_.plane())) {
    return true;
  }
  return top_.intersects(s)
    || left_.intersects(s)
    || right_.intersects(s)
    || front_.intersects(s)
    || back_.intersects(s)
    || bottom_.intersects(s);
}

Projection Box::project(const Vector& v) const {
  Projection p = top_.project(v);
  p = std::min(p, bottom_.project(v));
  p = std::min(p, left_.project(v));
  p = std::min(p, right_.project(v));
  p = std::min(p, front_.project(v));
  p = std::min(p, back_.project(v));
  return p;
}

Cylinder::Cylinder() {
}

Cylinder::Cylinder(const Vector& x0, const Vector& x1, float radius)
    : axis_(x0, x1), l_(sqrtf(axis_.l2_)), r_(radius), v_(axis_.x01_ / l_) {
}

bool Cylinder::intersects(const Sphere& s) const {
  /* Derived from ERIT section 2.5. */
  Vector pa = s.x_ - axis_.x0_;
  float pqdotpa = axis_.x01_.dot(pa);
  float rs1;

  /* P -> Q -> B */
  if (pqdotpa > axis_.l2_) {
    float f = pqdotpa - axis_.l2_;
    float d2 = (f * f) / axis_.l2_;
    if (d2 > s.r2_) {
      return false;
    }
    rs1 = sqrtf(s.r2_ - d2);
  }

  /* B -> P -> Q */
  else if (pqdotpa < 0.f) {
    float qpdotqa = -axis_.x01_.dot(s.x_ - axis_.x1_);
    float f = qpdotqa - axis_.l2_;
    float d2 = (f * f) / axis_.l2_;
    if (d2 > s.r2_) {
      return false;
    }
    rs1 = sqrtf(s.r2_ - d2);
  }

  /* P -> B -> Q */
  else {
    rs1 = s.r_;
  }

  float ld2 = axis_.x01_.cross(pa).squared() / axis_.l2_;
  float r = r_ + rs1;
  return ld2 <= (r * r);
}

Projection Cylinder::project(const Vector& p) const {
  /* Derived from ERIT section 2.5. */
  Vector pa = p - axis_.x0_;
  float pqdotpa = axis_.x01_.dot(pa);

  /* P -> Q -> B */
  if (pqdotpa > axis_.l2_) {
    Vector x = Plane(axis_.x1_, v_).project(p).x;

    Vector v = x - axis_.x1_;
    float l = v.length();
    if (l > r_) {
      float d = l - r_;
      Vector n = v / l;
      x -= n * d;
    }

    return Projection(x, (p - x).length(), v_);
  }

  /* B -> P -> Q */
  else if (pqdotpa < 0.f) {
    Vector x = Plane(axis_.x0_, v_).project(p).x;

    Vector v = x - axis_.x0_;
    float l = v.length();
    if (l > r_) {
      float d = l - r_;
      Vector n = v / l;
      x -= n * d;
    }

    return Projection(x, (p - x).length(), -v_);
  }

  /* P -> B -> Q */
  Vector b = axis_.x0_ + axis_.x01_ * pqdotpa / axis_.l2_;
  Vector v = p - b;
  float l = v.length();
  float d = l - r_;
  Vector n = v / l;
  return Projection(p - n * d, fabsf(d), (d < 0.f) ? -n : n);
}
