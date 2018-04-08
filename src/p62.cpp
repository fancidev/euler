/**
 * The cube, 41063625 (345^3), can be permuted to produce two other cubes:
 * 56623104 (384^3) and 66430125 (405^3). In fact, 41063625 is the smallest
 * cube which has exactly three permutations of its digits which are also
 * cube.
 *
 * Find the smallest cube for which exactly five permutations of its digits
 * are cube.
 */

#include <iostream>
#include <map>
#include "euler/digits.hpp"
#include "euler.h"

BEGIN_PROBLEM(62, solve_problem_62)
	PROBLEM_TITLE("Smallest cube where exactly five permutations of its digits are cube")
	PROBLEM_ANSWER("127035954683")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("K*log(K)")
	PROBLEM_SPACE_COMPLEXITY("K")
	PROBLEM_KEYWORDS("digits")
END_PROBLEM()

static void solve_problem_62()
{
#if 0
	const int max_perm = 3;
#else
	const int max_perm = 5;
#endif

	typedef long long int64_t;

	// Use a map { norm => cube } to store the smallest cube whose  
	// sorted digit sequence is equal to norm.
	std::map<int64_t,int64_t> norm_to_cube;

	// Use a map { cube => count } to store the number of integers
	// whose cube has the same digits as _cube_ after being sorted.
	std::map<int64_t,int> cube_counter;

	// Check all k^3 in turn.
	int last_ndigits = 0;
	for (int64_t k = 1; ; k++)
	{
		int64_t cube = k*k*k;
		int64_t norm = cube;
		int ndigits = euler::sort_digits<10>(norm, false);

		// If k^3 has more digits than the last iteration, check whether
		// the last batch of cubes contain 5-permutations.
		if (ndigits != last_ndigits)
		{
			for (auto it = cube_counter.cbegin(); it != cube_counter.cend(); ++it)
			{
				int64_t cube_repr = it->first;
				int count = it->second;
				if (count == max_perm)
				{
					std::cout << cube_repr << std::endl;
					return;
				}
			}

			// Reset statistics.
			last_ndigits = ndigits;
			norm_to_cube.clear();
			cube_counter.clear();
		}

		int64_t &first = norm_to_cube[norm];
		if (first == 0)
			first = cube;
		++cube_counter[first];
	}
}
