#include <cstdint>
#include <type_traits>
//#include "euler/matrix.hpp"
#include "euler/matrix_traits.hpp"
//#include "euler/matrix_adaptor.hpp"
#include "euler/matrix_algorithm.hpp"
#include "gtest/gtest.h"

TEST(matrix, matrix_traits)
{
  static_assert(std::is_same<
      typename euler::matrix_traits<int[3][5]>::value_type, int>::value,
      "matrix_traits failed");

  int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };

  static_assert(std::is_same<
      typename euler::matrix_traits<decltype(A)>::value_type, int>::value,
      "matrix_traits fails");
}

TEST(matrix, is_matrix)
{
  static_assert(
      std::is_base_of<std::true_type, euler::is_matrix<int[3][3]>>::value,
      "is_matrix fails");
  static_assert(
      std::is_base_of<std::false_type, euler::is_matrix<int>>::value,
      "is_matrix fails");

  static_assert(!euler::is_matrix<int>::value, "is_matrix fails");
  static_assert(!euler::is_matrix<int[2]>::value, "is_matrix fails");

  static_assert(euler::is_matrix<int[1][2]>::value, "is_matrix fails");
  static_assert(euler::is_matrix_v<int[1][2]>, "is_matrix_v fails");

  int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };

  static_assert(euler::is_matrix_v<decltype(A)>, "is_matrix_v fails");
}

TEST(matrix, extent)
{
  int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };

  EXPECT_EQ(3, euler::extent<0>(A));
  EXPECT_EQ(2, euler::extent<1>(A));
  EXPECT_EQ(0, euler::extent<2>(A));
}

TEST(matrix, common_extent)
{
  int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };
  int B[3][2] = { {1, 2}, {3, 4}, {5, 6} };

  EXPECT_EQ(3, euler::common_extent<0>(A, B));
  EXPECT_EQ(2, euler::common_extent<1>(A, B));
  EXPECT_EQ(0, euler::common_extent<2>(A, B));

  const int C[1][2] { };
  EXPECT_EQ(0, euler::common_extent<0>(A, C));
  EXPECT_EQ(2, euler::common_extent<1>(B, C));
  EXPECT_EQ(0, euler::common_extent<2>(A, C));
}

TEST(matrix, msum)
{
  int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };

  EXPECT_EQ(21, euler::msum(A));
}

TEST(matrix, meq)
{
  const int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };
  const int B[3][2] = { {1, 2}, {3, 4}, {5, 6} };
  EXPECT_EQ(true, euler::mall(euler::meq(A, B)));
}

TEST(matrix, madd)
{
  const int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };
  const int B[3][2] = { {7, 8}, {9, 0}, {3, 5} };
  int A_add_B[3][2] = { {8, 10}, {12, 4}, {8, 11} };

  EXPECT_EQ(true, euler::mall(euler::meq(A_add_B, euler::madd(A, B))));
}

TEST(matrix, msub)
{
  const int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };
  const int B[3][2] = { {7, 8}, {9, 0}, {3, 5} };
  int A_sub_B[3][2] = { {-6, -6}, {-6, 4}, {2, 1} };

  EXPECT_EQ(true, euler::mall(euler::meq(A_sub_B, euler::msub(A, B))));
}
