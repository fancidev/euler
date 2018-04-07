#include <iostream>
#include <algorithm>
#include <cmath>
#include "euler.h"

BEGIN_PROBLEM(42, solve_problem_42)
    PROBLEM_TITLE("How many triangle words does the list of common English words contain?")
    PROBLEM_ANSWER("162")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("N")
    PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static bool is_triangle_word(const char *s)
{
    // Compute the sum of alphabet positions.
    int k = 0;
    while (*s)
        k += ((*s++) - 'A' + 1);

    // Check whether k is triangle number, i.e. whether the equation
    // n^2 + n - 2k = 0 has an integer root. We just need to check
    // Delta = 1+8k is a square number.
    k = 1 + 8 * k;
    int r = (int)std::sqrt((double)k);
    return (r * r == k);
}

static const char *words[] = {
#include "p42-words.txt"
};

static void solve_problem_42()
{
    int size = sizeof(words) / sizeof(words[0]);
    auto count = std::count_if(&words[0], &words[size], is_triangle_word);
    std::cout << count << std::endl;
}
