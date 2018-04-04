#include <iostream>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(4, solve_problem_4)
	PROBLEM_TITLE("Find the largest palindrome made from the product of two 3-digit numbers")
	PROBLEM_ANSWER("906609")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("N^2 log(N)")
	PROBLEM_SPACE_COMPLEXITY("log(N)")
END_PROBLEM()

static void solve_problem_4()
{
	int max_n = 0;
	for (int a = 999; a >= 100; a--)
	{
		if (a * a <= max_n)
			break;
		for (int b = a; b >= 100; b--)
		{
			int n = a * b;
			if (n <= max_n)
				break;
			if (euler::is_palindromic<10>(n))
				max_n = n;
		}
	}
	std::cout << max_n << std::endl;
}
