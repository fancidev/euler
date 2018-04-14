/**
 * If the numbers 1 to 5 are written out in words: one, two, three, four, five, 
 * then there are 3 + 3 + 5 + 4 + 4 = 19 letters used in total.
 *
 * If all the numbers from 1 to 1000 (one thousand) inclusive were written out
 * in words, how many letters would be used? 
 *
 * NOTE: Do not count spaces or hyphens. For example, 342 (three hundred and 
 * forty-two) contains 23 letters and 115 (one hundred and fifteen) contains 20
 * letters. The use of "and" when writing out numbers is in compliance with 
 * British usage.
 */

#include <algorithm>
#include <iostream>
#include <string>
#include "euler.h"

BEGIN_PROBLEM(17, solve_problem_17)
  PROBLEM_TITLE("Write numbers in English words")
  PROBLEM_ANSWER("21124")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("n")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static std::string number_to_words(unsigned int n)
{
  static const char *map_20[20] = {  
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
  };
  static const char *map_tens[8] = {
    "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
  };

  if (n >= 10000)
  {
    return "not supported";
  }
  if (n == 0)
  {
    return map_20[0];
  }

  bool need_and = false;
  std::string s;
  if (n >= 1000)
  {
    s = map_20[n / 1000];
    s += " thousand";
    n %= 1000;
    need_and = true;
  }

  if (n >= 100)
  {
    if (s.size() > 0)
    {
      s += ' ';
    }
    s += map_20[n / 100];
    s += " hundred";
    n %= 100;
    need_and = true;
  }
  if (n == 0)
  {
    return s;
  }

  if (need_and)
  {
    s += " and ";
  }

  if (n < 20)
  {
    s += map_20[n];
  }
  else
  {
    s += map_tens[n / 10 - 2];
    n %= 10;
    if (n > 0)
    {
      s += '-';
      s += map_20[n];
    }
  }

  return s;
}

static void solve_problem_17()
{
  bool verbose = false;
  size_t total = 0;
  for (int i = 1; i <= 1000; i++)
  {
    std::string s = number_to_words(i);
    if (verbose)
    {
      std::cout << i << ": " << s << std::endl;
    }
    size_t letters = std::count_if(s.cbegin(), s.cend(), [](char c) {
      return (c >= 'a' && c <= 'z');
    });
    total += letters;
  }
  std::cout << total << std::endl;
}
