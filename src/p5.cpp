/**
 * 2520 is the smallest number that can be divided by each of the numbers 
 * from 1 to 10 without any remainder.
 *
 * What is the smallest positive number that is evenly divisible by all of 
 * the numbers from 1 to 20?
 */

#include <iostream>
#include "euler/gcd.hpp"
#include "euler.h"

BEGIN_PROBLEM(5, solve_problem_5)
	PROBLEM_TITLE("Smallest number divisible by each of the numbers 1 to 20")
	PROBLEM_ANSWER("232792560")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("N*ln(N)")
	PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static int lcm_range(int a, int b)
{
	int m = 1;
	for (int i = a; i <= b; i++) 
	{
		m = euler::lcm(m, i);
	}
	return m;
}

static void solve_problem_5()
{
	std::cout << lcm_range(1, 20) << std::endl;
}
