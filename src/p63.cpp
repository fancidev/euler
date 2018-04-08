/*
 * The 5-digit number, 16807=7^5, is also a fifth power. Similarly, the
 * 9-digit number, 134217728=8^9, is a ninth power.
 *
 * How many k-digit positive integers exist which are also an kth power?
 *
 * SOLUTION:
 *
 * Let n be a k-digit positive integer that is a kth power of integer d. Then
 *
 *   10^(k-1) <= n = d^k < 10^k
 *
 * which leads to 1 <= d <= 9 and
 *
 *                  1
 *   1 <= k <= -----------
 *              1 - lg(d)
 *
 * where lg denotes base-10 logarithm.
 *
 * ANSWER: 49
 */

#include <iostream>
#include <cmath>
#include "euler.h"

BEGIN_PROBLEM(63, solve_problem_63)
    PROBLEM_TITLE("Powerful digit counts")
    PROBLEM_ANSWER("49")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("")
    PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

static void solve_problem_63()
{
    int count = 0;
    for (int d = 1; d <= 9; d++)
    {
        count += static_cast<int>(1.0 / (1.0 - std::log10(d)));
    }
    std::cout << count << std::endl;
}
