/**
 * By listing the first six prime numbers: 2, 3, 5, 7, 11, and 13, 
 * we can see that the 6th prime is 13.
 *
 * What is the 10001st prime number?
 */

#include <iostream>
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(7, solve_problem_7)
	PROBLEM_TITLE("Find the 10001st prime")
	PROBLEM_ANSWER("104743")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("N*ln(N)+N*ln(ln(N))")
	PROBLEM_SPACE_COMPLEXITY("N*ln(N)+N*ln(ln(N))")
END_PROBLEM()

static void solve_problem_7()
{
#if 0
	const int N = 6;
#else
	const int N = 10001;
#endif

	int ub = euler::nth_prime_bounds(N).second;
	euler::prime_table<int> primes(ub);
	auto it = primes.begin();
	std::advance(it, N - 1);
	std::cout << *it << std::endl;
}
