/**
 * 145 is a curious number, as 1! + 4! + 5! = 1 + 24 + 120 = 145.
 *
 * Find the sum of all numbers which are equal to the sum of the factorial 
 * of their digits.
 *
 * Note: as 1! = 1 and 2! = 2 are not sums they are not included.
 *
 * ANSWER: 40730.
 *
 * NOTE: This problem is similar to problem 30.
 */

#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(34, solve_problem_34)
    PROBLEM_TITLE("Digit factorials")
    PROBLEM_ANSWER("40730")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("?")
    PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()


static const int fact[10] = { 1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880 };

static int sum_fact_digits(int n)
{
	int sum = 0;
	while (n > 0)
	{
		int d = n % 10;
		sum += fact[d];
		n /= 10;
	}
	return sum;
}

static void solve_problem_34()
{
	int total = 0;
	for (int n = 10; n <= fact[9]*7; n++)
	{
		if (n == sum_fact_digits(n))
		{
			//std::cout << n << std::endl;
			total += n;
		}
	}
	std::cout << total << std::endl;
}
