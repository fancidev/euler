/**
 * In the game of darts a player throws three darts at a target board which is
 * split into twenty equal sized sections numbered one to twenty.
 *
 * The score of a dart is determined by the number of the region that the dart
 * lands in. A dart landing outside the red/green outer ring scores zero. The
 * black and cream regions inside this ring represent single scores. However,
 * the red/green outer ring and middle ring score double and treble scores
 * respectively.
 *
 * At the centre of the board are two concentric circles called the bull
 * region, or bulls-eye. The outer bull is worth 25 points and the inner bull
 * is a double, worth 50 points.
 *
 * There are many variations of rules but in the most popular game the players
 * will begin with a score 301 or 501 and the first player to reduce their
 * running total to zero is a winner. However, it is normal to play a "doubles
 * out" system, which means that the player must land a double (including the
 * double bulls-eye at the centre of the board) on their final dart to win;
 * any other dart that would reduce their running total to one or lower means
 * the score for that set of three darts is "bust".
 *
 * When a player is able to finish on their current score it is called a
 * "checkout" and the highest checkout is 170: T20 T20 D25 (two treble 20s and
 * double bull).
 *
 * There are exactly eleven distinct ways to checkout on a score of 6:
 *
 *   D3
 *   D1 D2
 *   S2 D2
 *   D2 D1
 *   S4 D1
 *   S1 S1 D2
 *   S1 T1 D1
 *   S1 S3 D1
 *   D1 D1 D1
 *   D1 S2 D1
 *   S2 S2 D1
 *
 * Note that D1 D2 is considered different to D2 D1 as they finish on
 * different doubles. However, the combination S1 T1 D1 is considered the same
 * as T1 S1 D1.
 *
 * In addition we shall not include misses in considering combinations; for
 * example, D3 is the same as 0 D3 and 0 0 D3.
 *
 * Incredibly there are 42336 distinct ways of checking out in total.
 *
 * How many distinct ways can a player checkout with a score less than 100?
 */

#include <iostream>
#include "euler.h"

BEGIN_PROBLEM(109, solve_problem_109)
  PROBLEM_TITLE("Darts")
  PROBLEM_ANSWER("38182")
  PROBLEM_DIFFICULTY(1)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("combinatorics")
END_PROBLEM()

// Represents the outcome of throwing a single dart.
struct dart
{
  int multiple; // 0=miss, 1=single, 2=double, 3=triple
  int number;   // 0=miss, 1-20=ring, 25=bull's eye
  int score() const { return multiple * number; }
};

// There are in total 63 possible outcomes/scores for a single throw:
//
// Index  Outcome        Score
// -------------------------------------
// 0      miss           0
// 1-20   single number  1,2,3,...,19,20
// 21     single bull    25
// 22-41  double number  2,4,6,...,38,40
// 42     double bull    50
// 43-62  triple number  3,6,9,...,57,60

static const dart darts[63] = {
  /* 0 */
  {0,0},
  /* 1-20 */
  {1,1},{1,2},{1,3},{1,4},{1,5},{1,6},{1,7},{1,8},{1,9},{1,10},
  {1,11},{1,12},{1,13},{1,14},{1,15},{1,16},{1,17},{1,18},{1,19},{1,20},
  /* 21 */
  {1,25},
  /* 22-41 */
  {2,1},{2,2},{2,3},{2,4},{2,5},{2,6},{2,7},{2,8},{2,9},{2,10},
  {2,11},{2,12},{2,13},{2,14},{2,15},{2,16},{2,17},{2,18},{2,19},{2,20},
  /* 42 */
  {2,25},
  /* 43-62 */
  {3,1},{3,2},{3,3},{3,4},{3,5},{3,6},{3,7},{3,8},{3,9},{3,10},
  {3,11},{3,12},{3,13},{3,14},{3,15},{3,16},{3,17},{3,18},{3,19},{3,20},
};

static int count_ways_to_checkout(int min_score, int max_score)
{
  int count = 0;

  // Let the three dart outcomes be indexed (i,j,k). Must finish on a double.
  for (int k = 22; k <= 42; k++)
  {
    const int score3 = darts[k].score();

    // To eliminate duplicates, require the first outcome index (i) to be less
    // than or equal to the second outcome index (j).
    for (int j = 0; j < 63; j++)
    {
      const int score2 = darts[j].score();
      for (int i = 0; i <= j; i++)
      {
        const int score1 = darts[i].score();
        const int score = score1 + score2 + score3;
        if (score >= min_score && score <= max_score)
        {
          ++count;
        }
      }
    }
  }
  return count;
}

static void solve_problem_109()
{
  std::cout << count_ways_to_checkout(0, 99) << std::endl;
}
