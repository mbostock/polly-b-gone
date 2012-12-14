// -*- C++ -*-

#ifndef MBOSTOCK_VECTOR_H
#define MBOSTOCK_VECTOR_H

#include "math.h"

namespace mbostock {

  class Vector {
  public:
    inline Vector() : x(0.f), y(0.f), z(0.f) {}
    inline Vector(float x, float y) : x(x), y(y), z(0.f) {}
    inline Vector(float x, float y, float z) : x(x), y(y), z(z) {}

    inline Vector operator*(float k) const {
      return Vector(x * k, y * k, z * k);
    }

    inline Vector operator/(float k) const {
      return Vector(x / k, y / k, z / k);
    }

    inline Vector operator-(float k) const {
      return Vector(x - k, y - k, z - k);
    }

    inline Vector operator+(float k) const {
      return Vector(x + k, y + k, z + k);
    }

    inline Vector operator-() const {
      return Vector(-x, -y, -z);
    }

    inline Vector& operator*=(float k) {
      x *= k; y *= k; z *= k;
      return *this;
    }
    inline Vector& operator/=(float k) {
      x /= k; y /= k; z /= k;
      return *this;
    }
    inline Vector& operator-=(float k) {
      x -= k; y -= k; z -= k;
      return *this;
    }
    inline Vector& operator+=(float k) {
      x += k; y += k; z += k;
      return *this;
    }

    inline Vector operator-(const Vector& v) const {
      return Vector(x - v.x, y - v.y, z - v.z);
    }

    inline Vector operator+(const Vector& v) const {
      return Vector(x + v.x, y + v.y, z + v.z);
    }

    inline Vector operator*(const Vector& v) const {
      return Vector(x * v.x, y * v.y, z * v.z);
    }

    inline Vector operator/(const Vector& v) const {
      return Vector(x / v.x, y / v.y, z / v.z);
    }

    inline Vector& operator-=(const Vector& v) {
      x -= v.x; y -= v.y; z -= v.z;
      return *this;
    }

    inline Vector& operator+=(const Vector& v) {
      x += v.x; y += v.y; z += v.z;
      return *this;
    }

    inline Vector& operator*=(const Vector& v) {
      x *= v.x; y *= v.y; z *= v.z;
      return *this;
    }

    inline Vector& operator/=(const Vector& v) {
      x /= v.x; y /= v.y; z /= v.z;
      return *this;
    }

    inline bool operator==(const Vector& v) const {
      return (x == v.x) && (y == v.y) && (z == v.z);
    }

    inline bool operator!=(const Vector& v) const {
      return (x != v.x) || (y != v.y) || (z != v.z);
    }

    inline float dot(const Vector& v) const {
      return x * v.x + y * v.y + z * v.z;
    }

    inline Vector cross(const Vector& v) const {
      return Vector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    inline float length() const {
      return sqrtf(x * x + y * y + z * z);
    }

    inline float squared() const {
      return x * x + y * y + z * z;
    }

    inline Vector normalize() const {
      return *this / length();
    }

    static Vector min(const Vector& a, const Vector& b);
    static Vector max(const Vector& a, const Vector& b);
    static Vector randomVector(float k);

    static const Vector& ZERO();
    static const Vector& X();
    static const Vector& Y();
    static const Vector& Z();
    static const Vector& INF();

    float x;
    float y;
    float z;
  };

}

#endif
