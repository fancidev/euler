/**
 * Euler published the remarkable quadratic formula:
 *
 *   n^2 + n + 41
 *
 * It turns out that the formula will produce 40 primes for the consecutive 
 * values n = 0 to 39. However, when n = 40, 402 + 40 + 41 = 40(40 + 1) + 41
 * is divisible by 41, and certainly when n = 41, 41^2 + 41 + 41 is clearly 
 * divisible by 41.
 *
 * Using computers, the incredible formula  n^2 + 79n + 1601 was discovered, 
 * which produces 80 primes for the consecutive values n = 0 to 79. The product
 * of the coefficients, 79 and 1601, is 126479.
 *
 * Considering quadratics of the form:
 *
 *   n^2 + an + b, where |a| < 1000 and |b| < 1000
 *
 * Find the product of the coefficients, a and b, for the quadratic expression
 * that produces the maximum number of primes for consecutive values of n, 
 * starting with n = 0.
 */

#include <iostream>
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(27, solve_problem_27)
    PROBLEM_TITLE("Quadratic primes")
    PROBLEM_ANSWER("-59231")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(2)
    PROBLEM_TIME_COMPLEXITY("N^3 ln ln N")
    PROBLEM_SPACE_COMPLEXITY("N^2")
END_PROBLEM()

static void solve_problem_27()
{
	const int M = 1000;

    euler::prime_table<int> primes(2*M*M+M);
	int max_n = 0;
	int max_ab = 0;
	for (int a = -M+1; a < M; a++)
	{
		for (int b = 2; b < M; b++)
		{
			int y = b;
			int n = 0;
			while (y >= 2 && primes.test(y))
			{
				y += (2*n+1+a);
				n++;
			}
			if (n > max_n)
			{
				max_n = n;
				max_ab = a * b;
				//std::cout << "a = " << a << ", b = " << b << ", n = " << n << std::endl;
			}
		}
	}
	std::cout << max_ab << std::endl;
}
