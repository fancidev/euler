#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(28, solve_problem_28)
	PROBLEM_TITLE("Some of both diagonals in a 1001 by 1001 spiral")
	PROBLEM_ANSWER("669171001")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("1")
	PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static void solve_problem_28()
{
	const int n = 500;
	long long sum = 8LL*n*(n+1)*(2*n+1)/3 + 2LL*n*(n+1) + 4*n + 1;
	std::cout << sum << std::endl;
}
