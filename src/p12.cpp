/**
 * The sequence of triangle numbers is generated by adding the natural numbers. 
 * So the 7th triangle number would be 1 + 2 + 3 + 4 + 5 + 6 + 7 = 28. 
 * The first ten terms are:
 *
 *   1, 3, 6, 10, 15, 21, 28, 36, 45, 55, ...
 *
 * Let us list the factors of the first seven triangle numbers:
 *
 *    1: 1
 *    3: 1,3
 *    6: 1,2,3,6
 *   10: 1,2,5,10
 *   15: 1,3,5,15
 *   21: 1,3,7,21
 *   28: 1,2,4,7,14,28
 *
 * We can see that 28 is the first triangle number to have over five divisors.
 *
 * What is the value of the first triangle number to have over five hundred 
 * divisors?
 */

#include <iostream>
#include "euler/divisor.hpp"
#include "euler.h"

BEGIN_PROBLEM(12, solve_problem_12)
	PROBLEM_TITLE("The first triangle number with over 500 divisors")
	PROBLEM_ANSWER("76576500")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("n^2")
	PROBLEM_SPACE_COMPLEXITY("1")
	PROBLEM_KEYWORDS("triangle number,divisor")
END_PROBLEM()

static void solve_problem_12()
{
	for (int k = 1; ; k++)
	{
		int n = (k+1)*k/2;
		if (euler::count_divisors(n) > 500)
		{
			std::cout << n << std::endl;
			break;
		}
	}
}
