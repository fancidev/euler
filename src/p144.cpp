#include <iostream>
#include "euler/cartesian.hpp"
#include "euler.h"

BEGIN_PROBLEM(144, solve_problem_144)
  PROBLEM_TITLE("Multiple reflections of a laser beam")
  PROBLEM_ANSWER("354")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("N")
  PROBLEM_SPACE_COMPLEXITY("1")
  PROBLEM_KEYWORDS("geometry")
END_PROBLEM()

using namespace euler::cartesian2d;

static void solve_problem_144()
{
  ellipse<double> e(5.0, 10.0);
  point<double> p0(0.0, 10.1), p1(1.4, -9.6);

  int reflections = 1;
  for (; ; ++reflections)
  {
    point<double> p2 = mirror(p0, normal_line(e, p1));
    line<double> l(p1, p2);
    intersect(l, e, p0, p1);
    if (verbose())
    {
      std::cout << "Reflection: " << p1.x << ", " << p1.y << std::endl;
    }
    if (p1.y > 0 && fabs(p1.x) < 0.01)
    {
      break;
    }
  }
  std::cout << reflections << std::endl;
}
