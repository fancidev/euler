#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(2, solve_problem_2)
	PROBLEM_TITLE("Sum up even-valued terms in the Fibonacci sequence below 4,000,000")
	PROBLEM_ANSWER("4613732")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("log(N)")
	PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static void solve_problem_2()
{
	const int N = 4000000;
	int F_2 = 1, F_1 = 2, F;
	int sum = F_1;
	while ((F = F_2 + F_1) <= N)
	{
		if (F % 2 == 0)
			sum += F;
		F_2 = F_1;
		F_1 = F;
	}
	std::cout << sum << std::endl;
}
