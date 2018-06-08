/**
 * Let S(A) represent the sum of elements in set A of size n. We shall call it
 * a special sum set if for any two non-empty disjoint subsets, B and C, the
 * following properties are true:
 *
 *   (1) S(B) != S(C); that is, sums of subsets cannot be equal.
 *
 *   (2) If B contains more elements than C then S(B) > S(C).
 *
 * For example, { 81, 88, 75, 42, 87, 84, 86, 65 } is not a special sum set
 * because 65 + 87 + 88 = 75 + 81 + 84, whereas { 157, 150, 164, 119, 79, 159,
 * 161, 139, 158 } satisfies both rules for all possible subset pair
 * combinations and S(A) = 1286.
 *
 * Using sets.txt, a 4K text file with one-hundred sets containing seven to
 * twelve elements (the two examples given above are the first two sets in the
 * file), identify all the special sum sets, A[1], A[2], ..., A[k], and find
 * the value of S(A[1]) + S(A[2]) + ... + S(A[k]).
 *
 * NOTE: This problem is related to problems 103 and 106.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include "euler.h"

BEGIN_PROBLEM(105, solve_problem_105)
  PROBLEM_TITLE("Special subset sums: testing")
  PROBLEM_ANSWER("73702")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
END_PROBLEM()

template <class Iter>
static bool is_special_set(Iter first, Iter last)
{
  // Create a map {subset-sum => subset-size}
  std::map<int, int> sum_to_size;
  sum_to_size[0] = 0;
  for (Iter it = first; it != last; ++it)
  {
    const int a = *it;
    std::map<int, int> new_sum_to_size;

    // Add a to each existing sum
    for (const auto &kv: sum_to_size)
    {
      const int sum = kv.first;
      const int size = kv.second;
      if (sum_to_size.find(sum + a) != sum_to_size.end())
      {
        return false;
      }
      new_sum_to_size[sum + a] = size + 1;
    }

    // Merge new map into main map
    sum_to_size.insert(new_sum_to_size.begin(), new_sum_to_size.end());
  }

  // Verify that greater subset-sum corresponds to larget subset-size.
  int last_size = 0;
  for (const auto &kv: sum_to_size)
  {
    const int size = kv.second;
    if (!(last_size <= size))
    {
      return false;
    }
    last_size = size;
  }
  return true;
}

static void solve_problem_105()
{
  static const int sets[100][13] = {
#include "p105-sets.txt"
  };

  int total = 0;
  for (const auto &set: sets)
  {
    auto begin = std::begin(set);
    auto end = std::find(std::begin(set), std::end(set), 0);
    if (is_special_set(begin, end))
    {
      total += std::accumulate(begin, end, 0);
    }
  }
  std::cout << total << std::endl;
}
