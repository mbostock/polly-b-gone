// -*- C++ -*-

#include <iostream>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#include "shape.h"

using namespace mbostock;

static int returnCode = 0;

void assertTrue(bool condition, char* message, ...) {
  if (!condition) {
    va_list args;
    va_start(args, message);
    printf("assertion failed: ");
    vprintf(message, args);
    printf("\n");
    va_end(args);
  }
}

static void testLineXYZ() {
  printf("testLineXYZ...\n");
  LineSegment x(Vector(0, 0, 0), Vector(2, 0, 0));
  Projection px = x.project(Vector(1, 1, 0));
  assertTrue(px.length == 1, "px.length");
  assertTrue(px.x == Vector(1, 0, 0), "px.x");

  LineSegment y(Vector(1, 1, 1), Vector(1, 4, 1));
  Projection py = y.project(Vector(2, 1, 1));
  assertTrue(py.length == 1, "py.length");
  assertTrue(py.x == Vector(1, 1, 1), "py.x");

  LineSegment z(Vector(-2, -2, -2), Vector(-2, -2, -4));
  Projection pz = z.project(Vector(-2, -2, -2));
  assertTrue(pz.length == 0, "pz.length");
  assertTrue(pz.x == Vector(-2, -2, -2), "pz.x");
}

static void testLineX() {
  printf("testLineX...\n");
  LineSegment x(Vector(0, 0, 0), Vector(2, 0, 0));

  /* Colinear, before x0. */
  Projection p0 = x.project(Vector(-1, 0, 0));
  assertTrue(p0.length == 1, "p0.length");
  assertTrue(p0.x == Vector(0, 0, 0), "p0.x");

  /* Colinear, after x1. */
  Projection p1 = x.project(Vector(3, 0, 0));
  assertTrue(p1.length == 1, "p1.length");
  assertTrue(p1.x == Vector(2, 0, 0), "p1.x");

  /* Colinear, at x0. */
  Projection p2 = x.project(Vector(0, 0, 0));
  assertTrue(p2.length == 0, "p2.length");
  assertTrue(p2.x == Vector(0, 0, 0), "p2.x");

  /* Colinear, at x1. */
  Projection p3 = x.project(Vector(2, 0, 0));
  assertTrue(p3.length == 0, "p3.length");
  assertTrue(p3.x == Vector(2, 0, 0), "p3.x");

  /* Colinear, between x0 and x1. */
  for (float f = 0; f <= 2; f += .1f) {
    Projection p4 = x.project(Vector(f, 0, 0));
    assertTrue(p4.length == 0, "p4.length");
    assertTrue(p4.x == Vector(f, 0, 0), "p4.x");
  }

  /* Noncolinear, before x0. */
  Projection p5 = x.project(Vector(-1, -1, -1));
  assertTrue(p5.length == sqrtf(3), "p5.length");
  assertTrue(p5.x == Vector(0, 0, 0), "p5.x");

  /* Noncolinear, after x1. */
  Projection p6 = x.project(Vector(3, 1, 1));
  assertTrue(p6.length == sqrtf(3), "p6.length");
  assertTrue(p6.x == Vector(2, 0, 0), "p6.x");

  /* Noncolinear, between x0 and x1. */
  Projection p7 = x.project(Vector(1, 1, 1));
  assertTrue(p7.length == sqrtf(2), "p7.length");
  assertTrue(p7.x == Vector(1, 0, 0), "p7.x");
}

static void testPlaneXYZ() {
  printf("testPlaneXYZ...\n");
  Plane xz(Vector(0, 0, 0), Vector(0, 1, 0));
  Projection pxz = xz.project(Vector(1, 2, 3));
  assertTrue(pxz.length == 2, "pxz.length");
  assertTrue(pxz.x == Vector(1, 0, 3), "pxz.x");

  Plane xy(Vector(0, 0, 0), Vector(0, 0, 1));
  Projection pxy = xy.project(Vector(1, 2, 3));
  assertTrue(pxy.length == 3, "pxy.length");
  assertTrue(pxy.x == Vector(1, 2, 0), "pxy.x");

  Plane yz(Vector(0, 0, 0), Vector(1, 0, 0));
  Projection pyz = yz.project(Vector(1, 2, 3));
  assertTrue(pyz.length == 1, "pyz.length");
  assertTrue(pyz.x == Vector(0, 2, 3), "pyz.x");

  Projection pyz2 = yz.project(Vector(-1, 2, 3));
  assertTrue(pyz2.length == 1, "pyz2.length");
  assertTrue(pyz2.x == Vector(0, 2, 3), "pyz2.x");
}

static void testTriangle() {
  printf("testTriangle...\n");

  Triangle xy(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 0));
  assertTrue(xy.normal() == Vector(0, 0, 1), "xy.normal");

  /* At x0. */
  Projection p0 = xy.project(Vector(0, 0, 0));
  assertTrue(p0.length == 0, "p0.length");
  assertTrue(p0.x == Vector(0, 0, 0), "p0.x");

  /* At x1. */
  Projection p1 = xy.project(Vector(0, 1, 0));
  assertTrue(p1.length == 0, "p1.length");
  assertTrue(p1.x == Vector(0, 1, 0), "p1.x");

  /* At x2. */
  Projection p2 = xy.project(Vector(1, 0, 0));
  assertTrue(p2.length == 0, "p2.length");
  assertTrue(p2.x == Vector(1, 0, 0), "p2.x");

  /* In the plane of the triangle, x < x0. */
  Projection p3 = xy.project(Vector(-1, 0, 0));
  assertTrue(p3.length == 1, "p3.length");
  assertTrue(p3.x == Vector(0, 0, 0), "p3.x");

  /* In the plane of the triangle, x > x2. */
  Projection p4 = xy.project(Vector(2, 0, 0));
  assertTrue(p4.length == 1, "p4.length");
  assertTrue(p4.x == Vector(1, 0, 0), "p4.x");

  /* In the plane of the triangle, x = x2, y = x1. */
  Projection p5 = xy.project(Vector(1, 1, 0));
  assertTrue(p5.length == sqrtf(2) / 2, "p5.length");
  assertTrue(p5.x == Vector(.5, .5, 0), "p5.x");

  /* In front of the triangle (+z). */
  Projection p6 = xy.project(Vector(.3, .3, 1));
  assertTrue(p6.length == 1, "p6.length");
  assertTrue(p6.x == Vector(.3, .3, 0), "p6.x");
}

static void testQuad() {
  printf("testQuad...\n");

  Quad xy(Vector(0, 0, 0), Vector(1, 0, 0), Vector(1, 1, 0), Vector(0, 1, 0));
  assertTrue(xy.normal() == Vector(0, 0, 1), "xy.normal");

  /* At x0. */
  Projection p0 = xy.project(Vector(0, 0, 0));
  assertTrue(p0.length == 0, "p0.length");
  assertTrue(p0.x == Vector(0, 0, 0), "p0.x");

  /* At x1. */
  Projection p1 = xy.project(Vector(0, 1, 0));
  assertTrue(p1.length == 0, "p1.length");
  assertTrue(p1.x == Vector(0, 1, 0), "p1.x");

  /* At x2. */
  Projection p2 = xy.project(Vector(1, 0, 0));
  assertTrue(p2.length == 0, "p2.length");
  assertTrue(p2.x == Vector(1, 0, 0), "p2.x");

  /* In the plane of the quad, x < x0. */
  Projection p3 = xy.project(Vector(-1, 0, 0));
  assertTrue(p3.length == 1, "p3.length");
  assertTrue(p3.x == Vector(0, 0, 0), "p3.x");

  /* In the plane of the quad, x > x2. */
  Projection p4 = xy.project(Vector(2, 0, 0));
  assertTrue(p4.length == 1, "p4.length");
  assertTrue(p4.x == Vector(1, 0, 0), "p4.x");

  /* In the plane of the quad, x = x2, y = x1. */
  Projection p5 = xy.project(Vector(2, 2, 0));
  assertTrue(p5.length == sqrtf(2), "p5.length");
  assertTrue(p5.x == Vector(1, 1, 0), "p5.x");

  /* In the plane of the quad. */
  Projection p6 = xy.project(Vector(.5, .5, 0));
  assertTrue(p6.length == 0, "p6.length");
  assertTrue(p6.x == Vector(.5, .5, 0), "p6.x");

  /* In front of the quad (+z). */
  Projection p7 = xy.project(Vector(.3, .3, 1));
  assertTrue(p7.length == 1, "p7.length");
  assertTrue(p7.x == Vector(.3, .3, 0), "p6.x");
}

static void testRamp() {
  printf("testRamp...\n");

  Wedge r(Vector(-1, 0, .5),
          Vector(-.5, .25, .5),
          Vector(-.5, .25, -.5),
          Vector(-1, 0, -.5));

  Projection p0 = r.project(Vector(-0.694026, 0.05, 0.35));
  assertTrue(p0.length > 0, "p0.length");
}

static void testCylinderIntersects() {
  std::cout << "testCylinderIntersects...\n";
  Cylinder c(Vector(0, 0, 0), Vector(0, 2, 0), 1);

  Sphere s1(Vector(0, 3.1, 0), 1);
  assertTrue(!c.intersects(s1), "c.intersects(s1)");

  Sphere s2(Vector(0, 2.9, 0), 1);
  assertTrue(c.intersects(s2), "c.intersects(s2)");

  Sphere s3(Vector(0, 3, 0), .9);
  assertTrue(!c.intersects(s3), "c.intersects(s3)");

  Sphere s4(Vector(0, 3, 0), 1.1);
  assertTrue(c.intersects(s4), "c.intersects(s4)");

  Sphere s5(Vector(0, -1.1, 0), 1);
  assertTrue(!c.intersects(s5), "c.intersects(s5)");

  Sphere s6(Vector(0, -.9, 0), 1);
  assertTrue(c.intersects(s6), "c.intersects(s6)");

  Sphere s7(Vector(0, -1, 0), .9);
  assertTrue(!c.intersects(s7), "c.intersects(s7)");

  Sphere s8(Vector(0, -1, 0), 1.1);
  assertTrue(c.intersects(s8), "c.intersects(s8)");

  Sphere s9(Vector(2, 1, 0), .9);
  assertTrue(!c.intersects(s9), "c.intersects(s9)");

  Sphere s10(Vector(2, 1, 0), 1.1);
  assertTrue(c.intersects(s10), "c.intersects(s10)");

  Sphere s11(Vector(-2, 1, 0), .9);
  assertTrue(!c.intersects(s11), "c.intersects(s11)");

  Sphere s12(Vector(-2, 1, 0), 1.1);
  assertTrue(c.intersects(s12), "c.intersects(s12)");

  Sphere s13(Vector(0, 1, 2), .9);
  assertTrue(!c.intersects(s13), "c.intersects(s13)");

  Sphere s14(Vector(0, 1, 2), 1.1);
  assertTrue(c.intersects(s14), "c.intersects(s14)");

  Sphere s15(Vector(0, 1, -2), .9);
  assertTrue(!c.intersects(s15), "c.intersects(s15)");

  Sphere s16(Vector(0, 1, -2), 1.1);
  assertTrue(c.intersects(s16), "c.intersects(s16)");
}

static void testCylinderProject() {
  std::cout << "testCylinderProject...\n";
  Cylinder c(Vector(0, 0, 0), Vector(0, 2, 0), 1);

  Projection j1 = c.project(Vector(0, 3.1, 0));
  assertTrue(Vector(0, 2, 0) == j1.x, "c.project(p1).x");
  assertTrue(j1.length > 1.f && j1.length < 1.2f, "c.project(p1).length");
  assertTrue(Vector(0, 1, 0) == j1.normal, "c.project(p1).normal");

  Projection j2 = c.project(Vector(0, -1.1, 0));
  assertTrue(Vector(0, 0, 0) == j2.x, "c.project(p2).x");
  assertTrue(j2.length > 1.f && j2.length < 1.2f, "c.project(p2).length");
  assertTrue(Vector(0, -1, 0) == j2.normal, "c.project(p2).normal");

  Projection j3 = c.project(Vector(2.1, 1, 0));
  assertTrue(Vector(1, 1, 0) == j3.x, "c.project(p3).x");
  assertTrue(j3.length > 1.f && j3.length < 1.2f, "c.project(p3).length");
  assertTrue(Vector(1, 0, 0) == j3.normal, "c.project(p3).normal");

  Projection j4 = c.project(Vector(-2.1, 1, 0));
  assertTrue(Vector(-1, 1, 0) == j4.x, "c.project(p4).x");
  assertTrue(j4.length > 1.f && j4.length < 1.2f, "c.project(p4).length");
  assertTrue(Vector(-1, 0, 0) == j4.normal, "c.project(p4).normal");
}

int main(int argc, char** argv) {
  testLineXYZ();
  testLineX();
  testPlaneXYZ();
  testTriangle();
  testQuad();
  testRamp();
  testCylinderIntersects();
  testCylinderProject();
  return returnCode;
}
