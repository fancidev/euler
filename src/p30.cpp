/**
 * Surprisingly there are only three numbers that can be written as the sum 
 * of fourth powers of their digits:
 *
 *   1634 = 1^4 + 6^4 + 3^4 + 4^4
 *   8208 = 8^4 + 2^4 + 0^4 + 8^4
 *   9474 = 9^4 + 4^4 + 7^4 + 4^4
 * 
 * (As 1 = 1^4 is not a sum it is not included.)
 *
 * The sum of these numbers is 1634 + 8208 + 9474 = 19316.
 *
 * Find the sum of all the numbers that can be written as the sum of fifth
 * powers of their digits.
 *
 * ANSWER: 443839.
 *
 * Note: If the number of digits is equal to the power, such numbers are 
 * called "narcissistic numbers" in recreational mathematics.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "euler/imath.hpp"
#include "euler.h"

BEGIN_PROBLEM(30, solve_problem_30)
    PROBLEM_TITLE("Digit fifth powers")
    PROBLEM_ANSWER("443839")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("?")
    PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

// Find the maximum number of digits possible. Let this number be K, then
// it is the biggest number that satisfies 10^(K-1) <= K*(9^P).
static int max_possible_digits(int P)
{
	int nine_P = euler::ipow(9, P);
	int K = 1;
	for (int ten_K_1 = 1; ten_K_1 <= K*nine_P; ten_K_1 *= 10)
		K++;
	return K - 1;
}

static int sort_digits(int n)
{
	int digits[100];
	int count = 0;
	while (n > 0)
	{
		digits[count++] = n % 10;
		n /= 10;
	}

	std::sort(digits + 0, digits + count);
	n = 0;
	for (int i = 0; i < count; i++)
	{
		n = n * 10 + digits[i];
	}
	return n;
}

static int find_numbers(int ndigits, int current, int partial_sum, const int power[10])
{
	int start = current % 10;
	current *= 10;

	int sum = 0;
	if (ndigits == 1)
	{
		for (int d = start; d <= 9; d++)
		{
			int s = partial_sum + power[d];
			if (sort_digits(s) == current + d)
			{
				// std::cout << "Found " << s << std::endl;
				sum += s;
			}
		}
	}
	else
	{
		for (int d = start; d <= 9; d++)
		{
			sum += find_numbers(ndigits-1, current + d, partial_sum+power[d], power);
		}
	}
	return sum;
}

static void solve_problem_30()
{
	const int P = 5;

	int K = max_possible_digits(P);

	// Compute the power of d^P
	int power[10];
	for (int d = 0; d < 10; d++)
	{
		power[d] = euler::ipow(d, P);
	}

	// Find the sum of all such numbers, excluding 1.
	int sum = find_numbers(K, 0, 0, power);
	std::cout << (sum - 1) << std::endl;
}
