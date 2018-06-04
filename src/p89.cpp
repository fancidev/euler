/**
 * Rewrite Roman numerals in standard form.
 */

#include <iostream>
#include <sstream>
#include <string>
#include "euler/roman_numeral.hpp"
#include "euler.h"

BEGIN_PROBLEM(89, solve_problem_89)
  PROBLEM_TITLE("Develop a method to express Roman numerals in minimal form")
  PROBLEM_ANSWER("743")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("N")
  PROBLEM_SPACE_COMPLEXITY("1")
END_PROBLEM()

static void solve_problem_89()
{
  std::string inputs[1000] = {
#include "p89-roman.txt"
  };
  size_t savings = 0;
  for (const std::string &input: inputs)
  {
    int n;
    std::istringstream in(input);
    in >> euler::roman(n);
    std::ostringstream out;
    out << euler::roman(n);
    std::string output = out.str();
    savings += input.size() - output.size();
  }
  std::cout << savings << std::endl;
}
