/**
 * @defgroup datetime Date and Time Utilities
 *
 * @ingroup Library
 */

#ifndef EULER_DATETIME_HPP
#define EULER_DATETIME_HPP

#include <numeric>
#include <stdexcept>

namespace euler {

/**
 * Converts a date to an integer that is equal to the number of days since
 * January 0, 0000.
 *
 * @param year Year; must be between 0 and 9999.
 * @param month Month; must be between 1 and 12.
 * @param day Day; must be between 1 and the number of days in that month.
 *
 * @returns An integer that is equal to the number of days since January 0,
 *    0000. The return value is within the range 1 to 3652425.
 *
 * @exception std::invalid_argument if any argument is out of range.
 *
 * @remarks The behavior of this function is consistent with that of MATLAB's
 *    <c>datenum</c> function.
 *
 * @ingroup datetime
 */
inline int datenum(int year, int month, int day)
{
  const bool is_leap_year =
    (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);

  const int month_days[12] =
  {
    31, 28 + (is_leap_year? 1 : 0), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };

  if (!(year >= 0 && year <= 9999))
  {
    throw std::invalid_argument("year must be between 0 and 9999");
  }
  if (!(month >= 1 && month <= 12))
  {
    throw std::invalid_argument("month must be between 1 and 12");
  }
  if (!(day >= 1 && day <= month_days[month - 1]))
  {
    throw std::invalid_argument("day is out of range");
  }

  const int num_leap_years = (year+3)/4 - (year+99)/100 + (year+399)/400;
  int num_days = year * 365 + num_leap_years;
  num_days += std::accumulate(&month_days[0], &month_days[month - 1], 0);
  num_days += day;
  return num_days;
}

} // namespace euler

#endif // EULER_DATETIME_HPP
