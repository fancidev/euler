#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(1, solve_problem_1)
	PROBLEM_TITLE("Add natural numbers below 1,000 that are multiples of 3 or 5")
	PROBLEM_ANSWER("233168")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("1")
	PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

// Computes the sum of all multiples of m below N.
static int sum_of_multiples(int m, int N)
{
	int k = (N - 1) / m;
	int m2 = m * k;
	return (m + m2) * k / 2;
}

static void solve_problem_1()
{
	int N = 1000;
	int n = sum_of_multiples(3,N) + sum_of_multiples(5,N) - sum_of_multiples(15,N);
	std::cout << n << std::endl;
}
