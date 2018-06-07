/**
 * Three distinct points are plotted at random on a Cartesian plane, such that
 * a triangle is formed. Consider the following two triangles:
 *
 *   A(-340,495), B(-153,-910), C(835,-947)
 *   X(-175,41),  Y(-421,-714), Z(574,-645)
 *
 * It can be verified that triangle ABC contains the origin, whereas triangle
 * XYZ does not.
 *
 * Using triangles.txt, a text file containing the coordinates of one thousand
 * triangles, find the number of triangles for which the interior contains the
 * origin.
 *
 * Solution: see notes/p102.tex
 */

#include <algorithm>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(102, solve_problem_102)
  PROBLEM_TITLE("Count triangles that contain the origin")
  PROBLEM_ANSWER("228")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(3)
  PROBLEM_TIME_COMPLEXITY("n")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("geometry")
END_PROBLEM()

struct triangle
{
  int x1, y1, x2, y2, x3, y3;
};

static int det(int x1, int y1, int x2, int y2)
{
  return x1*y2 - x2*y1;
}

static bool contains_origin(const triangle &t)
{
  int d1 = det(t.x1, t.y1, t.x2, t.y2);
  int d2 = det(t.x2, t.y2, t.x3, t.y3);
  int d3 = det(t.x3, t.y3, t.x1, t.y1);
  return (d1 > 0 && d2 > 0 && d3 > 0) || (d1 < 0 && d2 < 0 && d3 < 0);
}

static void solve_problem_102()
{
  const triangle triangles[] = {
#include "p102-triangles.txt"
  };
  size_t count = std::count_if(
      std::begin(triangles), std::end(triangles), contains_origin);
  std::cout << count << std::endl;
}
