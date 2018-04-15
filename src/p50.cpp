/**
 * The prime 41, can be written as the sum of six consecutive primes:
 *
 *    41 = 2 + 3 + 5 + 7 + 11 + 13
 *
 * This is the longest sum of consecutive primes that adds to a prime below
 * one-hundred.
 *
 * The longest sum of consecutive primes below one-thousand that adds to a
 * prime, contains 21 terms, and is equal to 953.
 *
 * Which prime, below one-million, can be written as the sum of the most
 * consecutive primes?
 *
 * ANSWER: 997651.
 */

#include <iostream>
#include <vector>
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(50, solve_problem_50)
    PROBLEM_TITLE("Consecutive prime sum")
    PROBLEM_ANSWER("997651")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("?")
    PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

static void solve_problem_50()
{
#if 0
    const int N = 1000;
#else
    const int N = 1000000;
#endif

    // Find the primes below N.
    euler::prime_table<int> primes(N);

    // Compute cumulative sum of the primes.
    std::vector<int64_t> cumsum;
    {
        int64_t s = 0;
        cumsum.push_back(0);
        for (int p : primes)
        {
            cumsum.push_back(s += p);
        }
    }

    // Find the longest consecutive sum that is a prime.
    int max_length = 0;
    int max_p = 0;
    int count = static_cast<int>(cumsum.size());
    for (int i = 0; i < count; i++)
    {
        for (int j = i + max_length + 1; j < count; j++)
        {
            int64_t s = cumsum[j] - cumsum[i];
            if (s >= N)
            {
                break;
            }
            if (primes.test(static_cast<int>(s)))
            {
                max_length = j - i;
                max_p = static_cast<int>(s);
            }
        }
    }

    if (verbose())
    {
      std::cout << "Max P = " << max_p << ", max_length = "
                << max_length << std::endl;
    }
    std::cout << max_p << std::endl;
}
