/**
 * The series, 1^1 + 2^2 + 3^3 + ... + 10^10 = 10405071317.
 *
 * Find the last ten digits of the series, 1^1 + 2^2 + 3^3 + ... + 1000^1000.
 */

#include <cstdint>
#include <iostream>
#include "euler/residue.hpp"
#include "euler.h"

BEGIN_PROBLEM(48, solve_problem_48)
  PROBLEM_TITLE("Find the last ten digits of 1^1 + ... + 1000^1000")
  PROBLEM_ANSWER("9110846700")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N*log(N)")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static void solve_problem_48()
{
  const int64_t M = 10000000000;

  euler::residue<int64_t, M> sum(0);
  for (int n = 1; n <= 1000; n++)
  {
    sum += euler::residue<int64_t, M>(n) ^ n;
  }
  std::cout << sum << std::endl;
}
