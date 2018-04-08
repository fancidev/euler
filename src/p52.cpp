/**
 * It can be seen that the number, 125874, and its double, 251748, contain
 * exactly the same digits, but in a different order.
 *
 * Find the smallest positive integer, x, such that 2x, 3x, 4x, 5x, and 6x,
 * contain the same digits.
 *
 * Solution:
 *
 * Note that in order for x and 6x to have the same number of digits, the
 * first digit of x must be 1. In addition, the first digit of x, 2x, ...,
 * 6x must be different. So x must start with 1 and contain at least 6 digits.
 *
 * We use brute-force to test each of these numbers in turn until we find one.
 */

#include <iostream>
#include <algorithm>
#include <array>
#include <limits>
#include <functional>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(52, solve_problem_52)
	PROBLEM_TITLE("Find the smallest integer whose multiples contain the same digits")
	PROBLEM_ANSWER("142857")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("N*log(N)*log(log(N))")
	PROBLEM_SPACE_COMPLEXITY("log(N)")
END_PROBLEM()

// Reorder the digits of n from biggest to smallest.
static unsigned int digit_signature(unsigned int n)
{
	std::array<int, std::numeric_limits<unsigned int>::digits10+1> digits;
	auto it1 = digits.begin();
	auto it2 = std::copy(euler::rdigits<10>(n).begin(), euler::rdigits<10>(n).end(), it1);
	std::sort(it1, it2, std::greater<int>());
	return euler::from_digits<10, unsigned int>(it1, it2);
}

static void solve_problem_52()
{
	for (unsigned int n = 100000; ; n++)
	{
		unsigned int sig = digit_signature(n);
		if (sig == digit_signature(n*2)
			&& sig == digit_signature(n*3)
			&& sig == digit_signature(n*4)
			&& sig == digit_signature(n*5)
			&& sig == digit_signature(n*6))
		{
			std::cout << n << std::endl;
			break;
		}
	}
}
