/**
 * 1 Jan 1900 was a Monday.
 *
 * How many Sundays fell on the first of the month during the twentieth
 * century (1 Jan 1901 to 31 Dec 2000)?
 */

#include <iostream>
#include "euler/datetime.hpp"
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
  const int sunday = euler::datenum(1899, 12, 31);
  int num_sundays = 0;
  for (int year = 1901; year <= 2000; year++)
  {
    for (int month = 1; month <= 12; month++)
    {
      if ((euler::datenum(year, month, 1) - sunday) % 7 == 0)
      {
        num_sundays++;
      }
    }
  }
  std::cout << num_sundays << std::endl;
}
