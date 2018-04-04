/**
 * @defgroup PrimeTest Primality Testing
 * Routines to test whether an integer is prime.
 *
 * @ingroup Library
 *
 * This module contains routines to test the primality of an integer.
 */

#ifndef EULER_PRIME_TEST_HPP
#define EULER_PRIME_TEST_HPP

#include <cassert>
#include "modular.hpp"

namespace euler {

// Tests whether an integer is prime by trial division, given a list of
// the smallest primes to divide.
template <class T, class FwdIt>
bool is_prime(T n, FwdIt primes_begin, FwdIt primes_end)
{
	if (n <= 1)
		return false;
	if (n == 2)
		return true;
	if (n % 2 == 0)
		return false;

	// Test the number against each known prime.
	T p = 3;
	for (FwdIt it =  primes_begin; it != primes_end; ++it)
	{
		p = *it;
		if (n / p < p)
			return true;
		if (n % p == 0)
			return false;
	}

	// If the number is greater than maxp^2, we need to test
	// each odd number onwards.
	for (p |= 1; ; p += 2)
	{
		if (n / p < p)
			return true;
		if (n % p == 0)
			return false;
	}
}

#if 0
// Tests whether an integer is prime by trial division.
template <class T>
bool is_prime(T n)
{
	return is_prime(n, (T*)0, (T*)0);
}
#endif

/**
 * Tests whether an integer is prime by applying the deterministic
 * Miller-Rabin primality test.
 *
 * @param n The integer whose primality is tested. Must be positive.
 *      Must not be greater than 341550071728321.
 * @returns @c true if @c n is prime, false otherwise.
 * @timecomplexity <code>O(log<sub>2</sub>n)</code>.
 * @spacecomplexity Constant.
 * @remarks Note that the number @c 1 is not considered prime.
 *
 * @ingroup PrimeTest
 */
template <typename T>
bool is_prime(T n)
{
	return details::miller_rabin(n);
}

namespace details {

// Miller-Rabin test for n = 2^s*d+1 against a given witness.
// Witness must be less than n.
// @todo Evaluate the performance impact of @c modpow on the test.
template <typename T>
bool miller_rabin(T n, int s, T d, T witness)
{
	// Compute x = a^d mod n and check whether x == +/- 1 (mod n).
	T x = euler::modpow(witness, d, n);
	if (x == 1 || x == n - 1)
		return true;

	// Checks if any of (a^d)^2, (a^d)^4, ... == -1 (mod n).
	for (int j = 1; j < s; j++)
	{
		x = euler::modmul(x, x, n);
		if (x == n - 1)
			return true;
	}
	
	// Test failed.
	return false;
}

template <typename TArg>
bool miller_rabin(TArg number)
{
	return miller_rabin<TArg>(number, std::is_signed<TArg>());
}

template <typename TArg>
bool miller_rabin(TArg n, std::true_type /* is_signed */)
{
	static_assert(std::is_signed<TArg>::value, 
		"This function can only be called for signed types.");
	if (n < 0)
		return false;
	else
		return miller_rabin((typename std::make_unsigned<TArg>::type)n, std::false_type());
}

template <typename T>
bool miller_rabin(T n, std::false_type /* is_signed */)
{
	static_assert(!std::is_signed<T>::value, 
		"This function can only be called for unsigned types.");

	// Test the first few small numbers.
	if (n <= 1)
		return false;
	if (n == 2)
		return true;
	if (n % 2 == 0)
		return false;
	if (n < 9)
		return true;
	if (n % 3 == 0 || n % 5 == 0 || n % 7 == 0)
		return false;

	// A table of the candidate witness to test.
	static const unsigned long long threshold[4] = {
		//9080191ULL,
		4759123141ULL,
		2152302898747ULL,
		3474749660383ULL,
		341550071728321ULL,
	};
	static const unsigned int candidates[4][8] = {
		//{ 31, 73, 0 },
		{ 2, 7, 61, 0 },
		{ 2, 3, 5, 7, 11, 0 },
		{ 2, 3, 5, 7, 11, 13, 0 },
		{ 2, 3, 5, 7, 11, 13, 17, 0 },
	};

	// Find the suitable candidate set.
	int k = 0;
	for (; k < 4 && n >= threshold[k]; ++k);
	assert(k < 4);

	// Decompose n-1 = 2^s * d.
	// Complexity: O(log(2,n))
	int s = 0;
	T d = n - 1;
	while (d % 2 == 0)
	{
		d /= 2;
		++s;
	}

	// Check every potential witness. If any witness fails, n is composite.
	for (int i = 0; candidates[k][i] && candidates[k][i] < n; i++)
	{
		if (!miller_rabin(n, s, d, (T)candidates[k][i]))
			return false;
	}
	return true;
}

} // namespace details

/// Returns the smallest prime greater than a given number.
template <typename T>
T next_prime(T n)
{
	if (n < 2)
	{
		return 2;
	}
	else
	{
		T p = (n % 2 == 0)? n + 1 : n + 2;
		while (!is_prime(p))
			p += 2;
		return p;
	}
}

#if 0
static bool miller_rabin(unsigned int n)
{
	// Test the first few small numbers.
	if (n <= 1)
		return false;
	if (n == 2)
		return true;
	if (n % 2 == 0)
		return false;
	if (n < 9)
		return true;
	if (n % 3 == 0 || n % 5 == 0 || n % 7 == 0)
		return false;

	// Decompose n-1 = 2^s * d.
	// Complexity: O(log(2,n))
	int s = 0;
	unsigned int d = n - 1;
	while (d % 2 == 0)
	{
		d /= 2;
		++s;
	}

	// Check every potential witness. If any witness fails, n is composite.
	const unsigned int candidates[3] = { 2, 7, 61 };
	for (int i = 0; i < 3; i++)
	{
		unsigned int a = candidates[i];
		if (a >= n)
			continue;

		bool passed = false;

		// Compute x = a^d mod n and check whether x == +/- 1 (mod n).
		unsigned int x = euler::modpow(a, d, n);
		if (x == 1 || x == n - 1)
			passed = true;

		// Checks if any of (a^d)^2, (a^d)^4, ... == -1 (mod n)
		for (int j = 1; !passed && j < s; j++)
		{
			x = euler::modmul(x, x, n);
			if (x == n - 1)
				passed = true;
		}

		// Fails the test
		if (!passed)
			return false;
	}
	return true;
}
#endif

#if 0
void test_miller_rabin()
{
	for (int n = 1; n < 2000000; n += 2)
	{
		bool b1 = euler::is_prime(n);
		bool b2 = euler::miller_rabin(n);
		if (b1 != b2)
			std::cout << "Error: " << n << std::endl;
	}
}

void profile_miller_rabin()
{
	for (int n = 10000001; n < 20000000; n += 2)
	{
		bool b = euler::is_prime(n);
		//bool b = euler::miller_rabin(n);
		if (b != 0 && b != 1)
			std::cout << "a";
	}
}
#endif

} // namespace euler

#endif // EULER_PRIME_TEST_HPP
