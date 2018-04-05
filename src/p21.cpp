/**
 * Let d(n) be defined as the sum of proper divisors of n (numbers less than 
 * n which divide evenly into n). If d(a) = b and d(b) = a, where a <> b, then 
 * a and b are an amicable pair and each of a and b are called amicable numbers.
 *
 * For example, the proper divisors of 220 are 1, 2, 4, 5, 10, 11, 20, 22, 44, 55 
 * and 110; therefore d(220) = 284. The proper divisors of 284 are 1, 2, 4, 71 and 
 * 142; so d(284) = 220.
 *
 * Evaluate the sum of all the amicable numbers under 10000.
 */

#include <iostream>
#include <vector>
#include "euler/divisor.hpp"
#include "euler.h"

BEGIN_PROBLEM(21, solve_problem_21)
	PROBLEM_TITLE("Evaluate the sum of all amicable pairs under 10000")
	PROBLEM_ANSWER("31626")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("N*sqrt(N)")
	PROBLEM_SPACE_COMPLEXITY("N")
END_PROBLEM()

static void solve_problem_21()
{
	const int N = 10000;

	std::vector<int> d(N+1);
	for (int k = 1; k <= N; k++)
	{
		d[k] = euler::sum_divisors(k) - k;
	}

	int sum = 0;
	for (int k = 1; k <= N; k++)
	{
		if (d[k] != k && d[k] >= 1 && d[k] <= N && d[d[k]] == k)
		{
			// std::cout << "d(" << k << ") = " << d[k] << std::endl;
			sum += k;
		}
	}
	std::cout << sum << std::endl;
}