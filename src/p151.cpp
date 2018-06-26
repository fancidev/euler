/**
 * A printing shop runs 16 batches (jobs) every week and each batch requires a
 * sheet of special colour-proofing paper of size A5.
 *
 * Every Monday morning, the foreman opens a new envelope, containing a large
 * sheet of the special paper with size A1.
 *
 * He proceeds to cut it in half, thus getting two sheets of size A2. Then he
 * cuts one of them in half to get two sheets of size A3 and so on until he
 * obtains the A5-size sheet needed for the first batch of the week.
 *
 * All the unused sheets are placed back in the envelope.
 *
 *       +---------+---------+
 *       |         |         |
 *       |         |   A3    |
 *       |         |         |
 *   A1: |   A2    +----+----+
 *       |         |    | A5 |
 *       |         | A4 +----+
 *       |         |    | A5 |
 *       +---------+----+----+
 *
 * At the beginning of each subsequent batch, he takes from the envelope one
 * sheet of paper at random. If it is of size A5, he uses it. If it is larger,
 * he repeats the 'cut-in-half' procedure until he has what he needs and any
 * remaining sheets are always placed back in the envelope.
 *
 * Excluding the first and last batch of the week, find the expected number of
 * times (during each week) that the foreman finds a single sheet of paper in
 * the envelope.
 *
 * SOLUTION:
 *
 * Let f(n1,n2,n3,n4,n5) denote the expected number of times the foreman finds
 * a single sheet of paper in the envelope, starting with the configuration of
 * n1 pieces of A1, n2 pieces of A2, n3 pieces of A3, n4 pieces of A4 and n5
 * pieces of A5 sheets in the envelope, including the first and last batch.
 * The problem asks for f(1,0,0,0,0) - 2.
 *
 * We find a recurrence relation for f. Let n = n1 + n2 + n3 + n4 + n5 be the
 * total number of sheets in the envelope:
 *
 *   o If n1 > 0 and an A1 sheet is drawn, whose probability is (n1 / n), it
 *     will be cut in half repeatedly to produce 4 sheets of size A2..A5 each
 *     after consuming an extra A5 sheet;
 *
 *   o If n2 > 0 and an A2 sheet is drawn, whose probability is (n2 / n), it
 *     will be cut in half repeatedly to produce 3 sheets of size A3..A5 each
 *     after consuming an extra A5 sheet;
 *
 *   o If n3 > 0 and an A3 sheet is drawn, whose probability is (n3 / n), it
 *     will be cut in half repeatedly to produce 2 sheets of size A4..A5 each
 *     after consuming an extra A5 sheet;
 *
 *   o If n4 > 0 and an A4 sheet is drawn, whose probability is (n4 / n), it
 *     will be cut into two A5 sheets of which one is consumed and the other
 *     is left; and
 *
 *   o If n5 > 0 and an A5 sheet is drawn, whose probability is (n5 / n), it
 *     will be consumed directly.
 *
 * With this reasoning we find the recurrence relation
 *
 *   f(n1,n2,n3,n4,n5) = (n1/n) * f(n1-1,n2+1,n3+1,n4+1,n5+1)
 *                     + (n2/n) * f(n1,n2-1,n3+1,n4+1,n5+1)
 *                     + (n3/n) * f(n1,n2,n3-1,n4+1,n5+1)
 *                     + (n4/n) * f(n1,n2,n3,n4-1,n5+1)
 *                     + (n5/n) * f(n1,n2,n3,n4,n5-1)
 *                     + I(n==1)
 *
 * where the last term is 1 if there is a single sheet at inception, which
 * counts toward the expectation, or 0 otherwise. The boundary condition is
 * f(0,0,0,0,0) = 0.
 *
 * To evaluate f(1,0,0,0,0), we maintain a cache of f values given arguments
 * and apply the recurrence relation directly.
 */

#include <iostream>
#include <map>
#include <tuple>
#include "euler.h"

BEGIN_PROBLEM(151, solve_problem_151)
  PROBLEM_TITLE("Paper sheets of standard sizes: an expected-value problem")
  PROBLEM_ANSWER("0.464399")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("probability")
END_PROBLEM()

static double f(int n1, int n2, int n3, int n4, int n5)
{
  typedef std::tuple<int,int,int,int,int> Args;
  static std::map<Args,double> cache;

  const Args args(n1, n2, n3, n4, n5);

  auto it = cache.find(args);
  if (it != cache.end())
  {
    return it->second;
  }

  const int n = n1 + n2 + n3 + n4 + n5;
  double v = (n == 1)? 1.0 : 0.0;
  if (n1 > 0)
  {
    v += f(n1-1, n2+1, n3+1, n4+1, n5+1) * n1 / n;
  }
  if (n2 > 0)
  {
    v += f(n1, n2-1, n3+1, n4+1, n5+1) * n2 / n;
  }
  if (n3 > 0)
  {
    v += f(n1, n2, n3-1, n4+1, n5+1) * n3 / n;
  }
  if (n4 > 0)
  {
    v += f(n1, n2, n3, n4-1, n5+1) * n4 / n;
  }
  if (n5 > 0)
  {
    v += f(n1, n2, n3, n4, n5-1) * n5 / n;
  }

  cache[args] = v;
  return v;
}

static void solve_problem_151()
{
  std::cout.precision(6);
  std::cout << std::fixed << f(1,0,0,0,0) - 2 << std::endl;
}
