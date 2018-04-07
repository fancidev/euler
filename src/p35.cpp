/**
 * The number, 197, is called a circular prime because all rotations of the
 * digits: 197, 971, and 719, are themselves prime.
 *
 * There are thirteen such primes below 100: 2, 3, 5, 7, 11, 13, 17, 31, 37,
 * 71, 73, 79, and 97.
 *
 * How many circular primes are there below one million?
 *
 * ANSWER: 55.
 */

#include <iostream>
#include <vector>
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(35, solve_problem_35)
    PROBLEM_TITLE("Circular primes")
    PROBLEM_ANSWER("55")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("?")
    PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

static int rotate_digits(int n)
{
	int digits[100];
	int count = 0;
	do
	{
		digits[count++] = n % 10;
		n /= 10;
	}
	while (n > 0);

	n = digits[0];
	for (int i = count - 1; i >= 1; i--)
	{
		n = n * 10 + digits[i];
	}
	return n;
}

static void solve_problem_35()
{
	const int M = 1000000;

    euler::prime_table<int> primes(M);
	std::vector<bool> tested(M);
	int total = 0;

	for (auto it = primes.begin(); it != primes.end(); ++it)
	{
		int p = *it;
		if (tested[p])
			continue;

		int count = 1;
		int p2 = p;
		while ((p2 = rotate_digits(p2)) != p)
		{
			if (primes.test(p2))
			{
				tested[p2] = true;
				++count;
			}
			else
				break;
		}

		if (p2 == p) // satisfies condition
		{
			total += count;
		}
	}
	std::cout << total << std::endl;
}
