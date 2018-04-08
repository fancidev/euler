/**
 * By replacing the 1st digit of *3, it turns out that six of the nine
 * possible values: 13, 23, 43, 53, 73, and 83, are all prime.
 *
 * By replacing the 3rd and 4th digits of 56**3 with the same digit,
 * this 5-digit number is the first example having seven primes among
 * the ten generated numbers, yielding the family: 56003, 56113, 56333,
 * 56443, 56663, 56773, and 56993. Consequently 56003, being the first
 * member of this family, is the smallest prime with this property.
 *
 * Find the smallest prime which, by replacing part of the number (not
 * necessarily adjacent digits) with the same digit, is part of an eight
 * prime value family.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "euler/prime_table.hpp"
#include "euler/imath.hpp"
#include "euler.h"

BEGIN_PROBLEM(51, solve_problem_51)
	PROBLEM_TITLE("Eight different primes from by changing the same part of a number")
	PROBLEM_ANSWER("121313")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(1)
	PROBLEM_TIME_COMPLEXITY("")
	PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

static unsigned convert_decimal_to_bcd(unsigned n)
{
	unsigned bcd = 0, shift = 0;
	while (n > 0)
	{
		bcd |= (n % 10) << shift;
		n /= 10;
		shift += 4;
	}
	return bcd;
}

static unsigned mask_bcd_prime(unsigned p, unsigned mask)
{
	// the digits corresponding to the F in mask must be equal.
	int d = -1;
	unsigned int m = mask;
	for (int t = p; m != 0; m >>= 4, t >>= 4)
	{
		if (m & 0xf)
		{
			if (d == -1)
				d = t & 0xf;
			else if (d != (t & 0x0f))
				break;
		}
	}
	return (m == 0)? (p | mask) : 0;
}

static int print_prime_family(const std::vector<unsigned> &primes_bcd, unsigned mask, int members)
{
	// mask each number in the array with the given mask, and keep record
	// of how many numbers with the same mask are encountered.
	struct family_info
	{
		unsigned first_number;
		int count;
	};
	std::map<unsigned, family_info> families;
	for (auto it = primes_bcd.cbegin(); it != primes_bcd.cend(); ++it)
	{
		unsigned p = *it;
		unsigned pm = mask_bcd_prime(p, mask);
		family_info &f = families[pm];
		if (f.first_number == 0)
			f.first_number = p;
		++f.count;
	}

	// print the families with at least k members.
	int occurrence = 0;
	for (auto it = families.cbegin(); it != families.cend(); ++it)
	{
		unsigned family = it->first;
		const family_info &f = it->second;
		if (family != 0 && f.count >= members)
		{
			//std::cout << "Family " << std::hex << family << "(" << f.first_number << ") "
			//	<< "contains " << std::dec << f.count << " members." << std::endl;
			std::cout << std::hex << f.first_number << std::dec << std::endl;
			++occurrence;
		}
	}
	return occurrence;
}

static unsigned bin_mask_to_nibble_mask(unsigned binmask)
{
	unsigned m = 0;
	int shift = 0xf;
	while (binmask)
	{
		if (binmask & 1)
		{
			m |= shift;
		}
		shift <<= 4;
		binmask >>= 1;
	}
	return m;
}

static int find_prime_family(int digits, int members)
{
	// enumerate all prime numbers of the requested number of digits.
	int n = euler::ipow(10, digits);
	euler::prime_table<int> pp(n);
	std::vector<int> primes(pp.begin(), pp.end());
	auto it_begin = std::lower_bound(primes.cbegin(), primes.cend(), n / 10);
	auto it_end = primes.cend();

	// convert the prime numbers to bcd format to speed up further calculations.
	std::vector<unsigned> primes_bcd(it_end - it_begin);
	std::transform(it_begin, it_end, primes_bcd.begin(), convert_decimal_to_bcd);

	if (0)
	{
		for (auto it = primes_bcd.begin(); it != primes_bcd.end(); ++it)
			std::cout << std::hex << *it << ' ';
		std::cout << std::endl;
	}

	// enumerate all possible masks, and print all families that has at least
	// the requested number of members.
	int occurrence = 0;
	for (int mask = 1; mask < (1 << digits); mask++)
	{
		occurrence += print_prime_family(primes_bcd, bin_mask_to_nibble_mask(mask), members);
	}
	return occurrence;
}

static void solve_problem_51()
{
	for (int digits = 2; digits <= 6; digits++)
	{
		if (find_prime_family(digits, 8) > 0)
			break;
	}
}
