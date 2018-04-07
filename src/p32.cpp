/**
 * We shall say that an n-digit number is pandigital if it makes use of all 
 * the digits 1 to n exactly once; for example, the 5-digit number, 15234, 
 * is 1 through 5 pandigital.
 *
 * The product 7254 is unusual, as the identity, 39 * 186 = 7254, containing 
 * multiplicand, multiplier, and product is 1 through 9 pandigital.
 *
 * Find the sum of all products whose multiplicand/multiplier/product identity 
 * can be written as a 1 through 9 pandigital.
 * 
 * HINT: Some products can be obtained in more than one way so be sure to only 
 * include it once in your sum.
 *
 * ANSWER: 45228.
 */

#include <iostream>
#include <vector>
#include "euler.h"

BEGIN_PROBLEM(32, solve_problem_32)
    PROBLEM_TITLE("Pandigital products")
    PROBLEM_ANSWER("45228")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("?")
    PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

typedef unsigned short mask_t;

// Returns a mask of the digits in n. If n contains digit d (0 <= d <= 9), 
// then the d-th bit is set in the mask. If n contains duplicate digits,
// then the lowest bit (corresponding to digit 0) is set, and the rest 
// bits are undefined.
static mask_t get_mask(int n)
{
	mask_t m = 0;
	do
	{
		int d = n % 10;
		if (m & (1 << d))
		{
			m |= 1;
			break;
		}
		m |= (1 << d);
		if (d == 0)
		{
			break;
		}
		n /= 10;
	}
	while (n > 0);
	return m;
}

static void solve_problem_32()
{
	// Let a * b = c be pandigital from 1 to 9, a < b < c.
	// If a has n_1 digits, b has n_2 digits, c has n_3 digits, then
	// n1+n2-1 <= n3 <= n1+n2. We can find that
	// - if a has 1 digit, then b has four digits, and c has four digits
	// - if a has 2 digits, then b has three digits, and c has four digits
	std::vector<bool> is_product(9000);
	for (int a = 2; a < 100; a++)
	{
		mask_t ma = get_mask(a);
		if (ma & 1)
			continue;

		int min_b = (a < 10)? 1234 : 123;
		int max_b = (a < 10)? 9876 : 987;
		for (int b = min_b; b <= max_b; b++)
		{
			mask_t mb = get_mask(b);
			if (mb & 1)
				continue;
			if (ma & mb)
				continue;

			int c = a * b;
			int mc = get_mask(c);
			if (mc & 1)
				continue;
			if (mc & (ma | mb))
				continue;
			if ((mc | ma | mb) != 0x03fe) // 11 1111 1110
				continue;

			is_product[c - 1000] = true;
			//std::cout << a << " * " << b << " = " << c << std::endl;
		}
	}

	// Find the sum of all such products.
	int sum = 0;
	for (int n = 1000; n <= 9999; n++)
	{
		if (is_product[n - 1000])
			sum += n;
	}
	std::cout << sum << std::endl;
}
