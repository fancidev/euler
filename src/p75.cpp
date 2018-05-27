/**
 * It turns out that 12 cm is the smallest length of wire that can be bent to
 * form an integer-sided right angle triangle in exactly one way, but there
 * are many more examples.
 *
 *   12 cm: (3,4,5)
 *   24 cm: (6,8,10)
 *   30 cm: (5,12,13)
 *   36 cm: (9,12,15)
 *   40 cm: (8,15,17)
 *   48 cm: (12,16,20)
 *
 * In contrast, some lengths of wire, like 20 cm, cannot be bent to form an
 * integer-sided right angle triangle, and other lengths allow more than one
 * solution to be found; for example, using 120 cm it is possible to form
 * exactly three different integer-sided right angle triangles.
 *
 *   120 cm: (30,40,50), (20,48,52), (24,45,51)
 *
 * Given that L is the length of the wire, for how many values of
 * L <= 1,500,000 can exactly one integer sided right angle triangle be
 * formed?
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include "euler/integer_triangle.hpp"
#include "euler.h"

BEGIN_PROBLEM(75, solve_problem_75)
  PROBLEM_TITLE("Singular integer right triangles")
  PROBLEM_ANSWER("161667")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("integer triangle")
END_PROBLEM()

static int count_unique_triangles(int max_perimeter)
{
  // Create an array to store the number of right triangles of the
  // given perimeter. Since the perimeter must be an even number
  // and must be >= 2, the following mapping is used:
  //   perimeter -> perimeter / 2 - 1
  std::vector<int> triangle_count(max_perimeter / 2);

  euler::generate_right_triangles(max_perimeter, [&](int a, int b, int c)
  {
    int perimeter = a + b + c;
    for (int k = 1; perimeter * k <= max_perimeter; k++)
    {
      ++triangle_count[perimeter*k/2-1];
    }
  });

  // count the number of unique triangles.
  int unique_count = static_cast<int>(std::count(
      triangle_count.begin(), triangle_count.end(), 1));
  return unique_count;
}

static void solve_problem_75()
{
  int unique_count = count_unique_triangles(1500000);
  std::cout << unique_count << std::endl;
}
