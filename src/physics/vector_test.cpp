// -*- C++ -*-

#include <stdio.h>
#include <stdarg.h>

#include "vector.h"

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

static void testDefaultConstructor() {
  printf("testDefaultConstructor...\n");
  Vector v1;
  Vector v2;
  assertTrue(v1 == v2, "v1 != v2");
  assertTrue(v1.x == 0.f, "v1.x != 0.f");
  assertTrue(v1.y == 0.f, "v1.y != 0.f");
  assertTrue(v1.z == 0.f, "v1.z != 0.f");
}

static void testExplicitConstructor() {
  printf("testExplicitConstructor...\n");
  Vector v(1.f, 2.f, 3.f);
  assertTrue(v.x == 1.f, "v.x != 1.f");
  assertTrue(v.y == 2.f, "v.y != 2.f");
  assertTrue(v.z == 3.f, "v.z != 3.f");
}

static void testAssignment() {
  printf("testAssignment...\n");
  Vector v1(1.f, 2.f, 3.f);
  Vector v2(4.f, 5.f, 6.f);
  Vector v3(4.f, 5.f, 6.f);
  v1 = v2;
  assertTrue(v1 == v2, "v1 != v2");
  assertTrue(v1 == v3, "v1 != v3");
  assertTrue(v2 == v3, "v2 != v3");
}

static void testCopyConstructor() {
  printf("testCopyConstructor...\n");
  Vector v1(1.f, 2.f, 3.f);
  Vector v2(v1);
  assertTrue(v1 == v2, "v1 != v2");
  assertTrue(v2 == v1, "v2 != v1");
}

static void testEqualVector() {
  printf("testEqualVector...\n");
  Vector v1a(1.f, 2.f, 3.f);
  Vector v1b(1.f, 2.f, 3.f);
  Vector v2(1.f, 2.f, 4.f);
  Vector v3(1.f, 3.f, 3.f);
  Vector v4(2.f, 3.f, 3.f);
  assertTrue(v1a == v1b, "v1a != v1b");
  assertTrue(v1b == v1a, "v1b != v1a");
  assertTrue(!(v1a == v2), "v1a == v2");
  assertTrue(!(v1a == v3), "v1a == v3");
  assertTrue(!(v1a == v4), "v1a == v4");
}

static void testNotEqualVector() {
  printf("testNotEqualVector...\n");
  Vector v1a(1.f, 2.f, 3.f);
  Vector v1b(1.f, 2.f, 3.f);
  Vector v2(1.f, 2.f, 4.f);
  Vector v3(1.f, 3.f, 3.f);
  Vector v4(2.f, 3.f, 3.f);
  assertTrue(!(v1a != v1b), "v1a != v1b");
  assertTrue(!(v1b != v1a), "v1b != v1a");
  assertTrue(v1a != v2, "v1a == v2");
  assertTrue(v1a != v3, "v1a == v3");
  assertTrue(v1a != v4, "v1a == v4");
}

static void testMultiplyScalar() {
  printf("testMultiplyScalar...\n");
  Vector v1(1.f, 2.f, 3.f);
  Vector v2(2.f, 4.f, 6.f);
  assertTrue(v2 == (v1 * 2), "v2 != v1 * 2");
  assertTrue(v1 == (v2 * .5), "v1 != v2 * .5");
}

static void testDivideScalar() {
  printf("testDivideScalar...\n");
  Vector v1(1.f, 2.f, 3.f);
  Vector v2(2.f, 4.f, 6.f);
  assertTrue(v2 == (v1 / .5), "v2 != v1 / .5");
  assertTrue(v1 == (v2 / 2), "v1 != v2 / 2");
}

static void testAddScalar() {
  printf("testAddScalar...\n");
  Vector v1(1, 2, 3);
  assertTrue(Vector(2, 3, 4) == (v1 + 1), "v1 + 1");
  assertTrue(Vector(0, 1, 2) == (v1 + -1), "v1 + -1");
}

static void testSubtractScalar() {
  printf("testSubtractScalar...\n");
  Vector v1(1, 2, 3);
  assertTrue(Vector(0, 1, 2) == (v1 - 1), "v1 - 1");
  assertTrue(Vector(2, 3, 4) == (v1 - -1), "v1 - -1");
}

static void testMultiplyAssignmentScalar() {
  printf("testMultiplyAssignmentScalar...\n");
  Vector v1(1, 2, 3);
  v1 *= 2;
  assertTrue(Vector(2, 4, 6) == v1, "v1 * 2");
  v1 *= .5;
  assertTrue(Vector(1, 2, 3) == v1, "v1 * .5");
}

static void testDivideAssignmentScalar() {
  printf("testDivideAssignmentScalar...\n");
  Vector v1(1, 2, 3);
  v1 /= .5;
  assertTrue(Vector(2, 4, 6) == v1, "v1 / .5");
  v1 /= 2;
  assertTrue(Vector(1, 2, 3) == v1, "v1 / 2");
}

static void testAddAssignmentScalar() {
  printf("testAddAssignmentScalar...\n");
  Vector v1(1, 2, 3);
  v1 += 1;
  assertTrue(Vector(2, 3, 4) == v1, "v1 + 1");
  v1 += -1;
  assertTrue(Vector(1, 2, 3) == v1, "v1 + -1");
}

static void testSubtractAssignmentScalar() {
  printf("testSubtractAssignmentScalar...\n");
  Vector v1(1, 2, 3);
  v1 -= -1;
  assertTrue(Vector(2, 3, 4) == v1, "v1 - -1");
  v1 -= 1;
  assertTrue(Vector(1, 2, 3) == v1, "v1 - 1");
}

static void testAddVector() {
  printf("testAddVector...\n");
  Vector v1(1, 2, 3);
  Vector v2 = v1 * 2;
  Vector v3 = v1 + v2;
  assertTrue(Vector(3, 6, 9) == v3, "v1 + v1 * 2");
}

static void testSubtractVector() {
  printf("testSubtractVector...\n");
  Vector v1(1, 2, 3);
  Vector v2 = v1 * 2;
  Vector v3 = v1 - v2;
  assertTrue(Vector(-1, -2, -3) == v3, "v1 - v1 * 2");
}

static void testAddAssignmentVector() {
  printf("testAddAssignmentVector...\n");
  Vector v1(1, 2, 3);
  v1 += v1 * 2;
  assertTrue(Vector(3, 6, 9) == v1, "v1 + v1 * 2");
}

static void testSubtractAssignmentVector() {
  printf("testSubtractAssignmentVector...\n");
  Vector v1(1, 2, 3);
  v1 -= v1 * 2;
  assertTrue(Vector(-1, -2, -3) == v1, "v1 - v1 * 2");
}

static void testNegative() {
  printf("testNegative...\n");
  Vector v1(1, 2, 3);
  Vector v2(-1, -2, -3);
  assertTrue(v2 == -v1, "v2 != -v1");
}

static void testDotVector() {
  printf("testDotVector...\n");
  Vector v1(1, 2, 3);
  Vector v2(-2, -3, -4);
  float d = 1 * -2 + 2 * -3 + 3 * -4;
  assertTrue(v1.dot(v2) == d, "v1 dot v2");
}

static void testCrossVector() {
  printf("testCrossVector...\n");
  Vector x(1, 0, 0);
  Vector y(0, 1, 0);
  Vector z(0, 0, 1);
  assertTrue(x.cross(y) == z, "x cross y");
}

static void testLength() {
  printf("testLength...\n");
}

static void testNormalize() {
  printf("testNormalize...\n");
}

int main(int argc, char** argv) {
  testDefaultConstructor();
  testExplicitConstructor();
  testAssignment();
  testCopyConstructor();
  testEqualVector();
  testNotEqualVector();
  testMultiplyScalar();
  testDivideScalar();
  testAddScalar();
  testSubtractScalar();
  testMultiplyAssignmentScalar();
  testDivideAssignmentScalar();
  testAddAssignmentScalar();
  testSubtractAssignmentScalar();
  testAddVector();
  testSubtractVector();
  testAddAssignmentVector();
  testSubtractAssignmentVector();
  testNegative();
  testDotVector();
  testCrossVector();
  testLength();
  testNormalize();
  return returnCode;
}
