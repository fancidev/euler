/**
 * Using base_exp.txt, a 22K text file containing one thousand lines with a
 * base/exponent pair on each line, determine which line number has the
 * greatest numerical value.
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(99, solve_problem_99)
  PROBLEM_TITLE("Find the greatest base/exponent pair")
  PROBLEM_ANSWER("709")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("n")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

namespace {

struct base_exp
{
  int base;
  int exp;
};

bool operator<(const base_exp &a, const base_exp &b)
{
  return std::log(a.base) * a.exp < std::log(b.base) * b.exp;
}

} // namespace

static void solve_problem_99()
{
  const base_exp values[] = {
#include "p99-base_exp.txt"
  };
  auto it = std::max_element(std::begin(values), std::end(values));
  std::cout << (it - std::begin(values) + 1) << std::endl;
}
