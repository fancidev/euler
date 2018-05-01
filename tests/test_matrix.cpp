#include <cstdint>
#include <type_traits>
//#include "euler/matrix.hpp"
#include "euler/matrix_traits.hpp"
//#include "euler/matrix_adaptor.hpp"
//#include "euler/matrix_algo.hpp"
#include "gtest/gtest.h"

TEST(matrix, carray)
{
  static_assert(std::is_same<
      typename euler::matrix_traits<int[3][5]>::value_type, int>::value,
      "matrix_traits failed");

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
  static_assert(std::is_same<
      typename euler::matrix_traits<decltype(A)>::value_type, int>::value,
      "matrix_traits fails");

  EXPECT_EQ(3, euler::extent<0>(A));
  EXPECT_EQ(2, euler::extent<1>(A));
  EXPECT_EQ(0, euler::extent<2>(A));

//  EXPECT_EQ(21, msum(A));

#if 0
  int B[3][2] = { {7, 8}, {9, 0}, {3, 5} };
  int R[3][2];
  int A_add_B[3][2] = { {8, 10}, {12, 4}, {8, 11} };
  int A_sub_B[3][2] = { {-6, -6}, {-6, 4}, {2, 1} };
#endif

//  EXPECT_EQ(true, euler::mne_all(A, B));
//  EXPECT_EQ(true, euler::meq_all(A_add_B, madd(R, A, B)));
//  EXPECT_EQ(true, euler::meq_all(A_sub_B, msub(R, A, B)));
}
