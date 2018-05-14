/**
 * How many Sundays fell on the first of the month during the twentieth
 * century (1 Jan 1901 to 31 Dec 2000)?
 */

#include <iostream>
#include <ctime>
#include "euler.h"

BEGIN_PROBLEM(19, solve_problem_19)
  PROBLEM_TITLE("Counting Sundays")
  PROBLEM_ANSWER("171")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("calendar")
END_PROBLEM()

static void solve_problem_19()
{
  int num_sundays = 0;
  for (int year = 1901; year <= 2000; year++)
  {
    for (int month = 1; month <= 12; month++)
    {
      std::tm t = {0};
      t.tm_mday = 1;
      t.tm_mon = month - 1;
      t.tm_year = year - 1900;
      std::mktime(&t);
      if (t.tm_wday == 0)
      {
        num_sundays++;
      }
    }
  }
  std::cout << num_sundays << std::endl;
}
