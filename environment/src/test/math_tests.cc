#include "gtest/gtest.h"
#include "math/math.h"

TEST(MathTests, VectorAdd) {
  Vec2Int a = Vec2Int(0,1);
  Vec2Int b = Vec2Int(1,2);

  auto c = a + b;
  auto c2 = b + a;

  EXPECT_EQ(c[0], 1);
  EXPECT_EQ(c[1], 3);
  EXPECT_EQ(c2[0], 1);
  EXPECT_EQ(c2[1], 3);
}

TEST(MathTests, ConstAdd) {
  Vec2Int a = Vec2Int(0,1);
  int b = 1;

  auto c = a + b;
  auto c2 = b + a;

  EXPECT_EQ(c[0], 1);
  EXPECT_EQ(c[1], 2);
  EXPECT_EQ(c2[0], 1);
  EXPECT_EQ(c2[1], 2);
}

TEST(MathTests, VectorSub) {
  Vec2Int a = Vec2Int(0,1);
  Vec2Int b = Vec2Int(1,2);

  auto c = a - b;
  auto c2 = b - a;

  EXPECT_EQ(c[0], -1);
  EXPECT_EQ(c[1], -1);
  EXPECT_EQ(c2[0], 1);
  EXPECT_EQ(c2[1], 1);
}

TEST(MathTests, ConstSub) {
  Vec2Int a = Vec2Int(0,1);
  int b = 1;

  auto c = a - b;
  auto c2 = b - a;

  EXPECT_EQ(c[0], -1);
  EXPECT_EQ(c[1], 0);
  EXPECT_EQ(c2[0], 1);
  EXPECT_EQ(c2[1], 0);
}

TEST(MathTests, VectorMult) {
  Vec2Int a = Vec2Int(0,1);
  Vec2Int b = Vec2Int(1,2);

  auto c = a * b;
  auto c2 = b * a;

  EXPECT_EQ(c[0], 0);
  EXPECT_EQ(c[1], 2);
  EXPECT_EQ(c2[0], 0);
  EXPECT_EQ(c2[1], 2);
}

TEST(MathTests, ConstMult) {
  Vec2Int a = Vec2Int(0,1);
  int b = 2;

  auto c = a * b;
  auto c2 = b * a;

  EXPECT_EQ(c[0], 0);
  EXPECT_EQ(c[1], 2);
  EXPECT_EQ(c2[0], 0);
  EXPECT_EQ(c2[1], 2);
}

TEST(MathTests, VectorDiv) {
  Vec2Int a = Vec2Int(-4,4);
  Vec2Int b = Vec2Int(-4,-4);

  auto c = a / b;
  auto c2 = b / a;

  EXPECT_EQ(c[0], 1);
  EXPECT_EQ(c[1], -1);
  EXPECT_EQ(c2[0], 1);
  EXPECT_EQ(c2[1], -1);
}

TEST(MathTests, ConstDiv) {
  Vec2Int a = Vec2Int(2,-2);
  int b = 2;

  auto c = a / b;
  auto c2 = b / a;

  EXPECT_EQ(c[0], 1);
  EXPECT_EQ(c[1], -1);
  EXPECT_EQ(c2[0], 1);
  EXPECT_EQ(c2[1], -1);
}

TEST(MathTests, VectorCast) {
  Vec2 a = Vec2(2,-2);
  Vec2Int b = (Vec2Int)a;
  a = (Vec2)b;

  EXPECT_EQ(a[0],2);
  EXPECT_EQ(a[1], -2);
}

TEST(MathTests, VectorNorm) {
  Vec2Int a = Vec2Int(2,0);

  EXPECT_EQ(a.norm(),2);
}

TEST(MathTests, VectorSqNorm) {
  Vec2Int a = Vec2Int(2,0);

  EXPECT_EQ(a.sq_norm(),4);
}

TEST(MathTests, VectorDot) {
  Vec2Int a = Vec2Int(0,1);
  Vec2Int b = Vec2Int(2,3);

  auto c = a.dot(b);
  auto c2 = b.dot(a);

  EXPECT_EQ(c, 3);
  EXPECT_EQ(c2, 3);
}

TEST(MathTests, VectorSize) {
  Vec2Int a = Vec2Int(0,1);

  EXPECT_EQ(a.size(), 2);
}