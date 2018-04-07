/**
 * If p is the perimeter of a right angle triangle with integral length sides, 
 * {a,b,c}, there are exactly three solutions for p = 120.
 *
 *  {20,48,52}, {24,45,51}, {30,40,50}
 *
 * For which value of p <= 1000, is the number of solutions maximised?
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "euler/integer_triangle.hpp"
#include "euler.h"

BEGIN_PROBLEM(39, solve_problem_39)
	PROBLEM_TITLE("Find the perimeter with the most right angle triangles")
	PROBLEM_ANSWER("840")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("")
	PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

static void solve_problem_39()
{
#if 0
	const int max_p = 100;
#else
	const int max_p = 1000;
#endif

	std::vector<int> counter(max_p + 1);
	generate_right_triangles<int>(max_p, [&counter](int a, int b, int c) {
		int p = a + b + c;
		for (int pp = p; pp <= max_p; pp += p)
			++counter[pp];
	});
	std::cout << (std::max_element(counter.cbegin(), counter.cend()) - counter.cbegin()) << std::endl;
}
