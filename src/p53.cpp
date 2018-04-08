/**
 * Let C(n, r) = n! / (n - r)! / r!.
 *
 * How many, not necessarily distinct, values of C(n, r), for 1 ≤ n ≤ 100,
 * 0 ≤ r ≤ n, are greater than one-million?
 */

#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(53, solve_problem_53)
	PROBLEM_TITLE("Combinatoric selections")
	PROBLEM_ANSWER("4075")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("?")
	PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

static void solve_problem_53()
{
    int count = 0;
    for (int n = 1; n <= 100; n++)
    {
        int c_n_r = 1;
        for (int r = 1; r <= n / 2; r++)
        {
            c_n_r = c_n_r * (n - r + 1) / r;
            if (c_n_r > 1000000)
            {
                // Count r, r+1, ..., n-r terms
                count += (n - 2 * r + 1);
                break;
            }
        }
    }
    std::cout << count << std::endl;
}
