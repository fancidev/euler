#include <iostream>
#include "euler/prime_factor.hpp"
#include "euler.h"

BEGIN_PROBLEM(3, solve_problem_3)
	PROBLEM_TITLE("Find the largest prime factor of 600851475143")
	PROBLEM_ANSWER("6857")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("sqrt(N)")
	PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static long long get_largest_prime_factor(long long n)
{
	long long ret = 1;
	euler::prime_factorize(n, [&ret](long long p) {
		ret = p;
	});
	return ret;
}

static void solve_problem_3()
{
	const long long N = 600851475143LL;
	std::cout << get_largest_prime_factor(N) << std::endl;
}
