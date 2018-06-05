/**
 * By replacing each of the letters in the word CARE with 1, 2, 9, and 6
 * respectively, we form a square number: 1296 = 36^2. What is remarkable is
 * that, by using the same digital substitutions, the anagram, RACE, also
 * forms a square number: 9216 = 96^2. We shall call CARE (and RACE) a square
 * anagram word pair and specify further that leading zeroes are not
 * permitted, neither may a different letter have the same digital value as
 * another letter.
 *
 * Using words.txt, a 16K text file containing nearly two-thousand common
 * English words, find all the square anagram word pairs (a palindromic word
 * is NOT considered to be an anagram of itself).
 *
 * What is the largest square number formed by any member of such a pair?
 *
 * NOTE: All anagrams formed must be contained in the given text file.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "euler.h"

BEGIN_PROBLEM(98, solve_problem_98)
  PROBLEM_TITLE("Anagramic squares")
  PROBLEM_ANSWER("18769")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("?")
  PROBLEM_SPACE_COMPLEXITY("?")
  PROBLEM_KEYWORDS("combinatorics,permutation")
END_PROBLEM()

// Returns a string with characters sorted, e.g. CARE => ACER
static std::string sorted(const std::string &s)
{
  std::string result = s;
  std::sort(std::begin(result), std::end(result));
  return result;
}

using string_pair = std::pair<std::string, std::string>;

// Generates a string signature for an anagram pair which is invariant under
// isomorphism. For example, ("CARE", "RACE") becomes "ABCD:CBAD".
static std::string get_anagram_pair_signature(const string_pair &anagram_pair)
{
  char next = 'A';
  char map[256] = {0};
  std::string signature;
  for (const char c: anagram_pair.first)
  {
    const uint8_t k = static_cast<uint8_t>(c);
    if (map[k] == 0)
    {
      map[k] = next++;
    }
    signature.push_back(map[k]);
  }
  signature.push_back(':');
  for (const char c: anagram_pair.second)
  {
    const uint8_t k = static_cast<uint8_t>(c);
    signature.push_back(map[k]);
  }
  return signature;
}

// Returns a map (anagram pair signature) => (vector of anagram pairs with
// this signature) from a list of words.
template <class StringList>
static std::map<std::string, std::vector<string_pair>>
find_anagrams(const StringList &words)
{
  std::map<std::string, std::vector<string_pair>> anagrams;

  if (verbose())
  {
    std::cout << "# words: " << (std::end(words) - std::begin(words))
              << std::endl;
  }

  // Make a map (sorted-word) => (vector-of-words)
  std::map<std::string, std::vector<std::string>> anagram_groups;
  for (const std::string &word: words)
  {
    anagram_groups[sorted(word)].push_back(word);
  }

  // For each anagram pair, append an entry to the anagrams map
  for (const auto &kv: anagram_groups)
  {
    const std::vector<std::string> &anagram_group = kv.second;
    for (size_t i = 0; i < anagram_group.size(); i++)
    {
      for (size_t j = 0; j < anagram_group.size(); j++)
      {
        if (i != j)
        {
          string_pair anagram_pair = std::make_pair(
              anagram_group[i], anagram_group[j]);
          std::string signature = get_anagram_pair_signature(anagram_pair);
          anagrams[signature].push_back(anagram_pair);
        }
      }
    }
  }
  if (verbose())
  {
    std::cout << "# anagrams: " << anagrams.size() << std::endl;
    for (const auto &kv: anagrams)
    {
      std::cout << "[" << kv.first << "]";
      for (const string_pair &anagram_pair: kv.second)
      {
        std::cout << " (" << anagram_pair.first << ","
                  << anagram_pair.second << ")";
      }
      std::cout << std::endl;
    }
  }
  return anagrams;
}

static void solve_problem_98()
{
  const char * words[] = {
#include "p98-words.txt"
  };

  std::map<std::string, std::vector<string_pair>> anagram_words =
      find_anagrams(words);

  // The longest anagram has 9 letters. Generate all squares below 10^10.
  std::vector<std::string> squares;
  for (int i = 1; i * i < 1000000000; i++)
  {
    squares.push_back(std::to_string(i * i));
  }

  std::map<std::string, std::vector<string_pair>> anagram_squares =
      find_anagrams(squares);

  // Find all anagram square pairs with an isomorphic anagram word pair
  int largest = 0;
  for (const auto &kv: anagram_squares)
  {
    const std::string &signature = kv.first;
    const std::vector<string_pair> &anagram_pairs = kv.second;
    if (anagram_words.find(signature) != anagram_words.end())
    {
      for (const string_pair &anagram_pair: anagram_pairs)
      {
        largest = std::max(largest, std::stoi(anagram_pair.first));
        largest = std::max(largest, std::stoi(anagram_pair.second));
      }
    }
  }
  std::cout << largest << std::endl;
}
