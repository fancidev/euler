#include "euler/matrix.hpp"
#include "gtest/gtest.h"

TEST(matrix, default_ctor)
{
  // Elements are value-initialized in default constructor
  euler::matrix<int,3,2> a;
  EXPECT_EQ(0, a(0, 0));
  EXPECT_EQ(0, a(0, 1));
  EXPECT_EQ(0, a(1, 0));
  EXPECT_EQ(0, a(1, 1));
  EXPECT_EQ(0, a(2, 0));
  EXPECT_EQ(0, a(2, 1));
}

TEST(matrix, diagonal)
{
  euler::matrix<int,2,2> a = euler::matrix<int,2,2>::diagonal(5);
  EXPECT_EQ(5, a(0, 0));
  EXPECT_EQ(0, a(0, 1));
  EXPECT_EQ(0, a(1, 0));
  EXPECT_EQ(5, a(1, 1));
}

TEST(matrix, identity)
{
  auto a = euler::matrix<double,2,2>::identity();
  EXPECT_EQ(1.0, a(0, 0));
  EXPECT_EQ(0.0, a(0, 1));
  EXPECT_EQ(0.0, a(1, 0));
  EXPECT_EQ(1.0, a(1, 1));
}

TEST(matrix, all_of)
{
  euler::matrix<bool,2,1> a = {true, false};
  euler::matrix<bool,1,3> b = {true, true, true};
  EXPECT_EQ(false, euler::all_of(a));
  EXPECT_EQ(true, euler::all_of(b));
}

TEST(matrix, cmp_eq_mm)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> B = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> C = { {0, 2}, {3, 4}, {5, 6} };
  EXPECT_EQ(true, euler::all_of(A == B));
  EXPECT_EQ(false, euler::all_of(A == C));
}

TEST(matrix, add_mm)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> B = { {7, 8}, {9, 0}, {3, 5} };
  euler::matrix<int,3,2> C = { {8, 10}, {12, 4}, {8, 11} };
  EXPECT_EQ(true, euler::all_of(A + B == C));
}

TEST(matrix, add_ms)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> C = { {2, 3}, {4, 5}, {6, 7} };
  EXPECT_EQ(true, euler::all_of(A + 1 == C));
}

TEST(matrix, add_sm)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> C = { {2, 3}, {4, 5}, {6, 7} };
  EXPECT_EQ(true, euler::all_of(1 + A == C));
}

TEST(matrix, sub_mm)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> B = { {7, 8}, {9, 0}, {3, 5} };
  euler::matrix<int,3,2> C = { {-6, -6}, {-6, 4}, {2, 1} };
  EXPECT_EQ(true, euler::all_of(A - B == C));
}

TEST(matrix, sub_ms)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> C = { {0, 1}, {2, 3}, {4, 5} };
  EXPECT_EQ(true, euler::all_of(A - 1 == C));
}

TEST(matrix, sub_sm)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> C = { {4, 3}, {2, 1}, {0, -1} };
  EXPECT_EQ(true, euler::all_of(5 - A == C));
}

TEST(matrix, mul_mm)
{
  euler::matrix<int,4,3> A = { {1, 2, 3}, {3, 4, 5}, {5, 6, 7}, {7, 8, 9} };
  euler::matrix<int,3,2> B = { {4, 3}, {2, 1}, {0, -1} };
  euler::matrix<int,4,2> C = { {8, 2}, {20, 8}, {32, 14}, {44, 20} };
  EXPECT_EQ(true, euler::all_of(A * B == C));
}

TEST(matrix, mul_ms)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> C = { {2, 4}, {6, 8}, {10, 12} };
  EXPECT_EQ(true, euler::all_of(A * 2 == C));
}

TEST(matrix, mul_sm)
{
  euler::matrix<int,3,2> A = { {1, 2}, {3, 4}, {5, 6} };
  euler::matrix<int,3,2> C = { {2, 4}, {6, 8}, {10, 12} };
  EXPECT_EQ(true, euler::all_of(2 * A == C));
}

TEST(matrix, inv_1)
{
  euler::matrix<double,1,1> A = { 4.0 };
  euler::matrix<double,1,1> B = { 0.25 };
  EXPECT_EQ(true, euler::all_of(euler::inv(A) == B));
  EXPECT_EQ(true, euler::all_of(euler::inv(B) == A));
}

TEST(matrix, inv_2)
{
  euler::matrix<double,2,2> A = { {1.0, 2.0}, {3.0, 4.0} };
  euler::matrix<double,2,2> B = { {-2.0, 1.0}, {1.5, -0.5} };
  EXPECT_EQ(true, euler::all_of(euler::inv(A) == B));
  EXPECT_EQ(true, euler::all_of(euler::inv(B) == A));
}

TEST(matrix, pow)
{
  euler::matrix<int,2,2> A = { {1, -2}, {-3, 4} };
  euler::matrix<int,2,2> B = { {1069, -1558}, {-2337, 3406} };
  euler::matrix<int,2,2> I = euler::matrix<int,2,2>::identity();
  EXPECT_EQ(true, euler::all_of((A ^ 5) == B));
  EXPECT_EQ(true, euler::all_of((A ^ 0) == I));
}
