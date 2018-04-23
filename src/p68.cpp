#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include "euler.h"

BEGIN_PROBLEM(68, solve_problem_68)
  PROBLEM_TITLE("Magic 5-gon ring")
  PROBLEM_ANSWER("6531031914842725")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

// Let the outer ring be indexed 0,1,2,3,4
// Let the inner ring be indexed 1,2,3,4,0
// such that there are 5 routes:
// outer_0 - inner_1 - inner_2
// outer_1 - inner_2 - inner_3
// outer_2 - inner_3 - inner_4
// outer_3 - inner_4 - inner_0
// outer_4 - inner_0 - inner_1

static const int N = 5;

static std::string ring_to_string(int outer[N], int inner[N])
{
  std::stringstream ss;
  int k = static_cast<int>(std::min_element(outer + 0, outer + N) - outer);
  for (int i = k; i < k + N; i++)
  {
    ss << outer[i % N] << inner[(i + 1) % N] << inner[(i + 2) % N];
  }
  return ss.str();
}

static void solve_problem_68()
{
  // Initialize the index, outer, and inner ring.
  int index[N], outer[N], inner[N];
  for (int i = 0; i < N; i++)
  {
    index[i] = i;
    outer[i] = N + i + 1;
    inner[i] = i + 1;
  }

  // For each permutation of inner ring and outer ring, check if
  // the sum on each line sums to the same number.
  int counter = 0;
  std::string max_string;
  do
  {
    do
    {
      //cout << counter << ": outer: " << outer[0] << ' ' << outer[1] << ' ' << outer[2] << "   ";
      //cout << "inner: " << inner[0] << ' ' << inner[1] << ' ' << inner[2] << endl;
      int sum = outer[0] + inner[1] + inner[2];
      bool good = std::all_of(index + 1, index + N,
        [&outer, &inner, sum](int i) -> bool
      {
        return (outer[i] + inner[(i + 1) % N] + inner[(i + 2) % N]) == sum;
      });
      if (good)
      {
        std::string this_string = ring_to_string(outer, inner);
        if (verbose())
        {
          std::cout << this_string << std::endl;
        }
        max_string = std::max(this_string, max_string);
      }
      ++counter;
    }
    while (std::next_permutation(inner + 0, inner + N));
  }
  while (std::next_permutation(outer + 0, outer + N));

  std::cout << max_string << std::endl;
}
