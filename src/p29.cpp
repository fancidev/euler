/**
 * Consider all integer combinations of a^b for 2 <= a <= 5 and 2 <= b <= 5:
 *
 *   2^2=4,  2^3=8,   2^4=16,  2^5=32
 *   3^2=9,  3^3=27,  3^4=81,  3^5=243
 *   4^2=16, 4^3=64,  4^4=256, 4^5=1024
 *   5^2=25, 5^3=125, 5^4=625, 5^5=3125
 *
 * If they are then placed in numerical order, with any repeats removed, 
 * we get the following sequence of 15 distinct terms:
 *
 * 4, 8, 9, 16, 25, 27, 32, 64, 81, 125, 243, 256, 625, 1024, 3125
 *
 * How many distinct terms are in the sequence generated by a^b for 
 * 2 <= a <= 100 and 2 <= b <= 100?
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "euler/imath.hpp"
#include "euler.h"

BEGIN_PROBLEM(29, solve_problem_29)
  PROBLEM_TITLE("Distinct terms of the form a^b")
  PROBLEM_ANSWER("9183")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("M*(ln(M))^3")
  PROBLEM_SPACE_COMPLEXITY("M*ln(M)")
END_PROBLEM()

static int verify(int M)
{
  // Compute all a^b for a, b within [2,M].
  std::vector<double> b_log_a;
  b_log_a.reserve((M-1)*(M-1));
  for (int a = 2; a <= M; a++)
  {
    double log_a = log(a);
    for (int b = 2; b <= M; b++)
    {
      b_log_a.push_back(log_a * b);
    }
  }

  // Sort the results and remove duplicates.
  std::sort(b_log_a.begin(), b_log_a.end());
  int count = 1;
  for (int i = 1; i < (M-1)*(M-1); i++)
  {
    if (b_log_a[i] - b_log_a[i-1] > 1e-10)
    {
      count++;
    }
  }

  // Return the number of unique elements.
  return count;
}

// Count the number of unique products xy where x1 <= x <= x2, y1 <= y <= y2.
static int count_unique_product(int x_min, int x_max, int y_min, int y_max)
{
  if (x_min == x_max)
  {
    return y_max - y_min + 1;
  }
  if (y_min == y_max)
  {
    return x_max - x_min + 1;
  }

  std::vector<int> buffer;
  for (int x = x_min; x <= x_max; x++)
  {
    for (int y = y_min; y <= y_max; y++)
    {
      buffer.push_back(x * y);
    }
  }

  std::sort(buffer.begin(), buffer.end());
  auto it = std::unique(buffer.begin(), buffer.end());
  return static_cast<int>(it - buffer.begin());
}

// Count the number of unique powers a^b where 2 <= a,b <= M.
static int count_unique_power(int M)
{
  int total = 0;

  // Flags the numbers that are a perfect power of some smaller integer.
  std::vector<bool> perfect_power(M-1);

  // Caches the result of count_unique_product() for equal ilog(M,a).
  int last_count = 0;
  int last_log = 0;

  // Check each possible base in a^b.
  for (int a = 2; a <= M; a++)
  {
    if (!perfect_power[a-2])
    {
      // Update perfect power flags.
      for (int x = a * a; x <= M; x *= a)
      {
        perfect_power[x-2] = true;
      }

      // Count the number of distinct (a^x)^y such that
      // 2 <= a^x <= M, 2 <= y <= M.
      int L = euler::ilog(M,a);
      if (L != last_log)
      {
        last_count = count_unique_product(1, L, 2, M);
        last_log = L;
      }
      total += last_count;
    }
  }
  return total;
}

static void solve_problem_29()
{
#if 0
  //const int M = 5;
  const int M = 2000;
  bool verbose = true;
#else
  const int M = 100;
  bool verbose = false;
#endif

  if (verbose)
  {
    std::cout << "Total:  " << (M-1)*(M-1) << std::endl;
    std::cout << "Answer: " << count_unique_power(M) << std::endl;
    std::cout << "Verify: " << verify(M) << std::endl;
  }
  else
  {
    std::cout << count_unique_power(M) << std::endl;
  }
}
