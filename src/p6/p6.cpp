/**
 * The sum of the squares of the first 10 natural numbers is
 *
 *   1^2 + 2^2 + ... + 10^2 = 385.
 *
 * The square of the sum of the first 10 natural numbers is
 *
 *   (1 + 2 + ... + 10)^2 = 55^2 = 3025.
 *
 * Hence the difference between the sum of the squares of the first 10
 * natural numbers and the square of the sum is 3025 - 385 = 2640.
 *
 * Find the difference between the sum of the squares of the first 100
 * natural numbers and the square of the sum.
 *
 * SOLUTION:
 *
 * The formula for sum of squares is: n(n+1)(2n+1)/6. 
 * The formula for square of sums is: (n(n+1)/2)^2.
 * So the difference follows immediately.
 */

#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(6, solve_problem_6)
	PROBLEM_TITLE("Difference between the sum of squares and the square of sums")
	PROBLEM_ANSWER("25164150")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("1")
	PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static void solve_problem_6()
{
	int n = 100;
	int sum_of_squares = n*(n+1)*(2*n+1)/6;
	int square_of_sums = n*n*(n+1)*(n+1)/4;
	std::cout << (square_of_sums - sum_of_squares) << std::endl;
}
