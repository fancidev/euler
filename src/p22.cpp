/**
 * Using names.txt, a 46K text file containing over five-thousand first names,
 * begin by sorting it into alphabetical order. Then working out the
 * alphabetical value for each name, multiply this value by its alphabetical
 * position in the list to obtain a name score.
 *
 * For example, when the list is sorted into alphabetical order, COLIN, which is
 * worth 3 + 15 + 12 + 9 + 14 = 53, is the 938th name in the list. So, COLIN
 * would obtain a score of 938*53 = 49714.
 *
 * What is the total of all the name scores in the file?
 */

#include <iostream>
#include <algorithm>
#include <cstring>
#include "euler.h"

BEGIN_PROBLEM(22, solve_problem_22)
    PROBLEM_TITLE("Names scores")
    PROBLEM_ANSWER("871198282")
    PROBLEM_DIFFICULTY(1)
    PROBLEM_FUN_LEVEL(1)
    PROBLEM_TIME_COMPLEXITY("N*ln(N)")
    PROBLEM_SPACE_COMPLEXITY("N")
END_PROBLEM()

static const char *names[] = {
#include "p22-names.txt"
};

static bool compare_string(const char *s1, const char *s2)
{
	return strcmp(s1, s2) < 0;
}

static void solve_problem_22()
{
	const int N = sizeof(names) / sizeof(names[0]);
	std::sort(names + 0, names + N, compare_string);

	int total = 0;
	for (int i = 0; i < N; i++)
	{
		const char *s = names[i];
		int score = 0;
		for (; *s; s++)
		{
			score += (*s - 'A' + 1);
		}
		total += (i + 1) * score;
	}
	std::cout << total << std::endl;
}
