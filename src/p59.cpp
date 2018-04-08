#include <iostream>
#include <numeric>
#include "euler.h"

BEGIN_PROBLEM(59, solve_problem_59)
	PROBLEM_TITLE("Brute force attack on XOR cipher")
	PROBLEM_ANSWER("107359")
	PROBLEM_DIFFICULTY(1)
	PROBLEM_FUN_LEVEL(3)
	PROBLEM_TIME_COMPLEXITY("N+MLC")
	PROBLEM_SPACE_COMPLEXITY("C")
END_PROBLEM()

static const unsigned char cipher_text[] = {
#include "p59-cipher1.txt"
};

static const double letter_freq[27] = { // space, A, B, ..., Z
	13.59,  8.17,  1.49,  2.78,  4.25, 12.70,  2.23,  2.02,  6.09,
	 6.97,  0.15,  0.77,  4.03,  2.41,  6.75,  7.51,  1.93,  0.10,
	 5.99,  6.33,  9.06,  2.76,  0.98,  2.36,  0.15,  1.97,  0.07,
};

static char guess_passcode(int L, int k)
{
	const int N = sizeof(cipher_text) / sizeof(cipher_text[0]);

	// Count the frequency of the k,k+L,k+2L,... letter.
	int freq[256] = {0};
	for (int i = k; i < N; i += L)
	{
		++freq[cipher_text[i]];
	}

	// Try each possible passcode on this position.
	char best_passcode = 0;
	double best_match = 0;
	for (char p = 'a'; p <= 'z'; p++)
	{
		// Minimizing sum error of square is equivalent to maximizing the
		// inner product of expected frequencies and observed frequencies.
		double match = 0.0;
		for (int i = 0; i <= 26; i++)
		{
			// XOR p with the clear text to get the cipher text.
			double expected_freq = letter_freq[i];
			double observed_freq = (i == 0)? 
				freq[p^' '] : freq[p^('A'+i-1)] + freq[p^('a'+i-1)];
			match += observed_freq * expected_freq;
		}
		// std::cout << "Score for " << (char)p << " is " << match << std::endl;
		if (match > best_match)
		{
			best_match = match;
			best_passcode = p;
		}
	}
	return best_passcode;
}

static void solve_problem_59()
{
	const int N = sizeof(cipher_text) / sizeof(cipher_text[0]);
	const int L = 3; // length of password
	bool verbose = false;

	// Guess each char in the password.
	char password[L+1];
	for (int k = 0; k < L; k++)
	{
		password[k] = guess_passcode(L, k);
	}
	password[L] = 0;
	if (verbose)
		std::cout << password << std::endl;

	// Decode the cipher text.
	char clear_text[N+1];
	for (int i = 0; i < N; i++)
	{
		clear_text[i] = cipher_text[i] ^ password[i % L];
	}
	clear_text[N] = 0;
	if (verbose)
		std::cout << clear_text << std::endl;
	
	// Output the sum of ASCII of the clear text.
	int sum_of_ascii = std::accumulate(&clear_text[0], &clear_text[N], 0);
	std::cout << sum_of_ascii << std::endl;
}
