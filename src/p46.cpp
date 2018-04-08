/**
 * It was proposed by Christian Goldbach that every odd composite number can
 * be written as the sum of a prime and twice a square.
 *
 *  9 =  7 + 2*1^2
 * 15 =  7 + 2*2^2
 * 21 =  3 + 2*3^2
 * 25 =  7 + 2*3^2
 * 27 = 19 + 2*2^2
 * 33 = 31 + 2*1^2
 *
 * It turns out that the conjecture was false.
 *
 * What is the smallest odd composite that cannot be written as the sum of a
 * prime and twice a square?
 *
 * ANSWER: 5777
 */

#include <iostream>
#include <cmath>
#include "euler/prime_table.hpp"
#include "euler.h"

BEGIN_PROBLEM(46, solve_problem_46)
    PROBLEM_TITLE("Goldbach's other conjecture")
    PROBLEM_ANSWER("5777")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("?")
    PROBLEM_SPACE_COMPLEXITY("?")
END_PROBLEM()

void solve_problem_46()
{
    const int N = 1000000;
    euler::prime_table<int> primes(N);

    // For each odd composite number below N, try decompose it into the sum
    // of a prime and twice a square.
    for (int n = 9; n < N; n += 2)
    {
        // skip prime number itself
        if (primes.test(n))
            continue;

        // test each possible square number
        bool found = false;
        for (int b = (int)sqrt((double)(n-3)/2); b >= 1; b--)
        {
            int a = n - 2*b*b;
            if (primes.test(a))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            //cout << "First non-conforming number is " << n << endl;
            std::cout << n << std::endl;
            break;
        }
    }
}
