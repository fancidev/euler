#include <cstdint>
#include <type_traits>
//#include "euler/matrix.hpp"
#include "euler/matrix_traits.hpp"
//#include "euler/matrix_adaptor.hpp"
#include "euler/matrix_algorithm.hpp"
#include "gtest/gtest.h"

#define MYTEST(functionName, testAspect) \
  TEST(matrix ## __ ## functionName, testAspect)

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

  EXPECT_EQ(3u, euler::extent<0>(A));
  EXPECT_EQ(2u, euler::extent<1>(A));
  EXPECT_EQ(0u, euler::extent<2>(A));
}

TEST(matrix, common_extent)
{
  int A[3][2] = { {1, 2}, {3, 4}, {5, 6} };
  int B[3][2] = { {1, 2}, {3, 4}, {5, 6} };

  EXPECT_EQ(3u, euler::common_extent<0>(A, B));
  EXPECT_EQ(2u, euler::common_extent<1>(A, B));
  EXPECT_EQ(0u, euler::common_extent<2>(A, B));

  const int C[1][2] { };
  EXPECT_EQ(0u, euler::common_extent<0>(A, C));
  EXPECT_EQ(2u, euler::common_extent<1>(B, C));
  EXPECT_EQ(0u, euler::common_extent<2>(A, C));
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

MYTEST(make_lazy_vector, basic)
{
  // basic usage
  auto v = euler::make_lazy_vector(3, [](size_t i) { return i; });
  EXPECT_EQ(0, v[0]);
  EXPECT_EQ(1, v[1]);
  EXPECT_EQ(2, v[2]);
}

MYTEST(make_lazy_vector, mutable)
{
  // elements in lazy vector are mutable if functor returns mutable object
  int a[3] = { 1, 2, 3 };
  auto v = euler::make_lazy_vector(3, [&a](size_t i) -> int&
  {
    return a[i];
  });
  v[1] = 4;
  EXPECT_EQ(1, v[0]);
  EXPECT_EQ(4, v[1]);
  EXPECT_EQ(3, v[2]);
}

MYTEST(make_lazy_vector, void)
{
  // functor may return void
  int a[3] = { 1, 2, 3 };
  auto v = euler::make_lazy_vector(3, [&a](size_t i) { ++a[i]; });
  static_assert(std::is_same<void, decltype(v[0])>::value, "");
  v[1];
  v[1];
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(4, a[1]);
  EXPECT_EQ(3, a[2]);
}

MYTEST(make_lazy_vector, cv_irrelevant_for_lambda)
{
  // cv-qualifier of vector does not propagate to lambda
  int a[3] = { 1, 2, 3 };
  const auto &v = euler::make_lazy_vector(3, [&a](size_t i) -> int&
  {
    return a[i];
  });
  v[1] = 4;
  EXPECT_EQ(1, v[0]);
  EXPECT_EQ(4, v[1]);
  EXPECT_EQ(3, v[2]);
}

MYTEST(make_lazy_vector, cv_irrelevant_for_functor)
{
  // cv-qualifier of vector does not propagate to functor
  struct C
  {
    int *a;
    double *b;
    int &operator()(size_t i) { return a[i]; }
    double &operator()(size_t i) const { return b[i]; }
  };

  int a[3] = { 1, 2, 3 };
  double b[3] = { 1.25, 3.75, 5.50 };

  auto v1 = euler::make_lazy_vector(3, C{a, b});
  v1[2] *= 6;
  EXPECT_EQ(18, a[2]);
  EXPECT_EQ(5.50, b[2]);

  const auto &v2 = euler::make_lazy_vector(3, C{a, b});
  v2[1] *= 2;
  EXPECT_EQ(4, a[1]);
  EXPECT_EQ(3.75, b[1]);
}

MYTEST(make_lazy_vector, cv_irrelevant_for_reference)
{
  // cv-qualifier of vector is not propagated to reference of functor
  struct C
  {
    int *a;
    double *b;
    int &operator()(size_t i) { return a[i]; }
    double &operator()(size_t i) const { return b[i]; }
  };

  int a[4] = { 1, 2, 3, 4 };
  double b[4] = { 1.25, 3.75, 5.50, 8.125 };

  C c{a, b};
  auto v1 = euler::make_lazy_vector<C&>(4, c);
  v1[2] *= 6;
  EXPECT_EQ(18, a[2]);
  EXPECT_EQ(5.50, b[2]);

  const auto &v2 = euler::make_lazy_vector<C&>(4, c);
  v2[1] *= 2;
  EXPECT_EQ(4, a[1]);
  EXPECT_EQ(3.75, b[1]);

  auto v3 = euler::make_lazy_vector<const C &>(4, c);
  v3[3] *= 7;
  EXPECT_EQ(4, a[3]);
  EXPECT_EQ(8.125*7, b[3]);

  const auto &v4 = euler::make_lazy_vector<const C &>(4, c);
  v4[0] *= 11;
  EXPECT_EQ(1, a[0]);
  EXPECT_EQ(1.25*11, b[0]);
}

MYTEST(make_lazy_vector, reference)
{
  // functor may be stored by reference (already implicitly tested above)
  struct C
  {
    explicit C(int k) : k(k) { }
    C(const C &) = delete;
    C(C&&) = delete;
    int operator()(size_t i) { return i * k; }
  private:
    int k;
  };
  C c(3);
  auto v = euler::make_lazy_vector(100, c);
  EXPECT_EQ(15, v[5]);
}

MYTEST(make_lazy_matrix, basic)
{
  // basic usage of lazy matrix
  auto a = euler::make_lazy_matrix(2, 3, std::plus<size_t>());
  EXPECT_EQ(0, a[0][0]);
  EXPECT_EQ(1, a[0][1]);
  EXPECT_EQ(2, a[0][2]);
  EXPECT_EQ(1, a[1][0]);
  EXPECT_EQ(2, a[1][1]);
  EXPECT_EQ(3, a[1][2]);
}

MYTEST(make_lazy_matrix, mutable)
{
  // matrix elements are mutable if functor returns mutable object
  int a[2][3] = { {10, 20, 30}, {40, 50, 60} };
  auto v = euler::make_lazy_matrix(2, 3, [&a](size_t i, size_t j) -> int&
  {
    return a[i][j];
  });
  v[1][2] /= 5;
  EXPECT_EQ(50, v[1][1]);
  EXPECT_EQ(12, v[1][2]);
}

MYTEST(make_lazy_matrix, cv_irrelevant_for_lambda)
{
  // cv-qualifier of matrix does not propagate to lambda
  int a[2][3] = { {10, 20, 30}, {40, 50, 60} };
  const auto &v = euler::make_lazy_matrix(2, 3,
      [&a](size_t i, size_t j) -> int&
      {
        return a[i][j];
      });
  v[1][2] /= 5;
  EXPECT_EQ(50, a[1][1]);
  EXPECT_EQ(12, a[1][2]);
}

MYTEST(make_lazy_matrix, cv_irrelevant_for_functor)
{
  // cv-qualifier of matrix does not propagate to functor
  int a[5] = { 1, 2, 3, 4, 5 };
  double b[5] = { 1.25, 3.75, 5.50, 8.125, 2.00 };
  struct C
  {
    int *a;
    double *b;
    int &operator()(size_t i, size_t j) { return a[i + j]; }
    double &operator()(size_t i, size_t j) const { return b[i + j]; }
  };

  auto v1 = euler::make_lazy_matrix(3, 3, C{a, b});
  v1[2][1] *= 6;
  EXPECT_EQ(24, a[3]);
  EXPECT_EQ(8.125, b[3]);

  const auto &v2 = euler::make_lazy_matrix(3, 3, C{a, b});
  v2[2][2] *= 2;
  EXPECT_EQ(10, a[4]);
  EXPECT_EQ(2.0, b[4]);
}

MYTEST(make_lazy_matrix, reference)
{
  // functor may be stored by reference (already implicitly tested above)
  struct C
  {
    explicit C(int k) : k(k) { }
    C(const C &) = delete;
    C(C&&) = delete;
    int operator()(size_t i) { return i * k; }
  private:
    int k;
  };
  C c(3);
  auto v = euler::make_lazy_vector(100, c);
  EXPECT_EQ(15, v[5]);
}
