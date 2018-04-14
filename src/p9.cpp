/**
 * A Pythagorean triplet is a set of three natural numbers, a b c, for which
 *
 *   a^2 + b^2 = c^2.
 *
 * For example, 3^2 + 4^2 = 5^2.
 *
 * There exists exactly one Pythagorean triplet for which a + b + c = 1000.
 * Find the product abc.
 */

#include <iostream>
#include "euler/integer_triangle.hpp"
#include "euler.h"

BEGIN_PROBLEM(9, solve_problem_9)
  PROBLEM_TITLE("Find the unique Pythagorean triplet that sums to 1000")
  PROBLEM_ANSWER("31875000")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N^2")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("pythagorean")
END_PROBLEM()

static void solve_problem_9()
{
  euler::generate_right_triangles<int>(1000, [](int a, int b, int c) {
    if (1000 % (a+b+c) == 0)
    {
      int x = 1000/(a+b+c);
      std::cout << a*b*c*x*x*x << std::endl;
    }
  });
}
