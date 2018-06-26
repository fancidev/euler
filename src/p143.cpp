/**
 * Let ABC be a triangle with all interior angles being less than 120 degrees.
 * Let X be any point inside the triangle and let XA = p, XB = q, XC = r.
 * Define T to be the point that minimizes p + q + r. We call this point the
 * Torricelli point.
 *
 * If the sum is minimised and a, b, c, p, q and r are all positive integers
 * we shall call triangle ABC a Torricelli triangle. For example, a = 399,
 * b = 455, c = 511 is an example of a Torricelli triangle, with p+q+r=784.
 *
 * Find the sum of all distinct values of p + q + r <= 120000 for Torricelli
 * triangles.
 *
 * SOLUTION:
 *
 * Properties of the Torricelli point are summarized in the Wikipedia page
 * https://en.wikipedia.org/wiki/Fermat_point. In particular, for a triangle
 * whose interior angles are all less than 120 degrees, the three angles
 * formed by connecting its Torricelli point to its vertices are all 120
 * degrees.
 *
 * Therefore we first generate all integer triangles with an 120-degree angle
 * and then enumerate ways to combine three of them to form a large triangle.
 * Note that without loss of generality we only need to align the shortest
 * side; this helps to reduce the search space.
 */

#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include "euler/integer_triangle.hpp"
#include "euler.h"

BEGIN_PROBLEM(143, solve_problem_143)
  PROBLEM_TITLE("Investigating the Torricelli point of a triangle")
  PROBLEM_ANSWER("30758397")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("geometry")
END_PROBLEM()

static void solve_problem_143()
{
#if 1
  const int max_pqr = 120000;
#else
  const int max_pqr = 10000;
#endif

  // obtuse_sides is a pair (p, q) where p <= q that stores the side length
  // of the two shorter (obtuse) sides of a 120-degree triangle.
  typedef std::pair<int, int> obtuse_sides;

  // obtuse_triangles contains all "relevant" 120-degree triangles; see below.
  std::vector<obtuse_sides> obtuse_triangles;

  // Enumerate all 120-degree triangles where the sum of the shorter sides is
  // less than max_pqr.
  euler::generate_120_degree_triangles((int)0,
      [max_pqr, &obtuse_triangles](int a, int b, int c) -> bool
  {
    const int L = a + b + c;
    if (b > c)
    {
      std::swap(b, c);
    }
    for (int bb = b, cc = c; bb + cc < max_pqr; bb += b, cc += c)
    {
      obtuse_triangles.push_back(obtuse_sides(bb, cc));
    }
    return (L < 2 * max_pqr);
  });

  // Sort the obtuse triangles by (p, q) to enable grouping and binary search.
  std::sort(obtuse_triangles.begin(), obtuse_triangles.end());

  // For each group (p, *) that shares a common shortest side length, test
  // whether two sides form a 120-degree integer triangle.
  const size_t n = obtuse_triangles.size();
  std::vector<bool> sum_exists(max_pqr + 1);
  for (size_t i = 0; i < n; i++)
  {
    const int p = obtuse_triangles[i].first;
    const int q = obtuse_triangles[i].second;
    for (size_t j = i; j < n && obtuse_triangles[j].first == p; j++)
    {
      const int r = obtuse_triangles[j].second;
      const int pqr = p + q + r;
      if (pqr > max_pqr)
      {
        continue;
      }

      if (std::binary_search(obtuse_triangles.begin() + j,
          obtuse_triangles.end(), obtuse_sides(q, r)))
      {
        sum_exists[pqr] = true;
        if (verbose())
        {
          std::cout << "p,q,r = " << p << "," << q << "," << r << std::endl;
        }
      }
    }
  }

  int sum = 0;
  for (int L = 1; L <= max_pqr; L++)
  {
    if (sum_exists[L])
    {
      sum += L;
    }
  }
  std::cout << sum << std::endl;
}
