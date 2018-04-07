/**
 * A unit fraction contains 1 in the numerator. The decimal representation 
 * of the unit fractions with denominators 2 to 10 are given:
 *
 * 1/2 =  0.5 
 * 1/3 =  0.(3) 
 * 1/4 =  0.25 
 * 1/5 =  0.2 
 * 1/6 =  0.1(6) 
 * 1/7 =  0.(142857) 
 * 1/8 =  0.125 
 * 1/9 =  0.(1) 
 * 1/10 =  0.1 
 *
 * Where 0.1(6) means 0.166666..., and has a 1-digit recurring cycle. 
 * It can be seen that 1/7 has a 6-digit recurring cycle.
 *
 * Find the value of d < 1000 for which 1/d contains the longest recurring 
 * cycle in its decimal fraction part.
 */

#include <iostream>
#include <vector>
#include "euler.h"

BEGIN_PROBLEM(26, solve_problem_26)
    PROBLEM_TITLE("Reciprocal cycles")
    PROBLEM_ANSWER("983")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("N^2")
    PROBLEM_SPACE_COMPLEXITY("N")
END_PROBLEM()

// Get the recurring cycle of p/q (in base 10). If p/q doesn't recur, 
// returns 0.
// Time complexity: O(q)
// Space complexity: 4*q bytes
static unsigned int get_recurring_cycle(unsigned int p, unsigned int q)
{
	std::vector<unsigned int> pos(q);
	unsigned int r = 1;
	for (p %= q; p != 0; r++)
	{
		if (pos[p] > 0)
			return r - pos[p];
		pos[p] = r;
		p = (p * 10) % q;
	}
	return 0;
}

static void solve_problem_26()
{
	unsigned int max_cycle = 0;
	unsigned int max_q = 0;
	for (unsigned int q = 1; q < 1000; q++)
	{
		unsigned int cycle = get_recurring_cycle(1, q);
		if (cycle > max_cycle)
		{
			max_cycle = cycle;
			max_q = q;
		}
		//std::cout << "q = " << q << ", cycle = " << cycle << std::endl;
	}
#if 0
	std::cout << "Longest recurring cycle is cycle(" << max_q << ") = " 
		<< max_cycle << std::endl;
#else
    std::cout << max_q << std::endl;
#endif
}