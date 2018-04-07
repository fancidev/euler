/**
 * Take the number 192 and multiply it by each of 1, 2, and 3:
 *
 *  192 * 1 = 192
 *  192 * 2 = 384
 *  192 * 3 = 576
 *
 * By concatenating each product we get the 1 to 9 pandigital, 192384576. 
 * We will call 192384576 the concatenated product of 192 and (1,2,3).
 * 
 * The same can be achieved by starting with 9 and multiplying by 
 * 1, 2, 3, 4, and 5, giving the pandigital, 918273645, which is the 
 * concatenated product of 9 and (1,2,3,4,5).
 *
 * What is the largest 1 to 9 pandigital 9-digit number that can be formed 
 * as the concatenated product of an integer with (1,2, ... , n) where n > 1?
 */

#include <iostream>
#include <vector>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(38, solve_problem_38)
	PROBLEM_TITLE("Largest 1-9 pandigital number as concatenated product")
	PROBLEM_ANSWER("932718654")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("")
	PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

static void solve_problem_38()
{
	bool verbose = false;
	int max_number = 0;
	std::vector<int> digits;
	for (int n = 2; n <= 9; n++)
	{
		for (int m = 1; ; m++)
		{
			// Concatenate the digits of m*(1,2,...,n).
			digits.clear();
			for (int k = 1; k <= n; k++)
			{
				std::copy(euler::digits<10>(k*m).begin(), euler::digits<10>(k*m).end(), 
					std::back_inserter(digits));
			}
			if (digits.size() > 9)
			{
				break;
			}
			if (euler::is_pandigital<10>(digits.cbegin(), digits.cend()))
			{
				int number = euler::from_digits<10,int>(digits.cbegin(), digits.cend());
				max_number = std::max(max_number, number);
				if (verbose)
					std::cout << number << " = (1.." << n << ") * " << m << std::endl;
			}
		}
	}
	std::cout << max_number << std::endl;
}
