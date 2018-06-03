/**
 * In the game of Monopoly, 40 squares are layed out in the following order in
 * a cycle:
 *
 *        00   01   02   03   04   05   06   07   08   09   10
 *      +----+----+----+----+----+----+----+----+----+----+----+
 *   00 | GO | A1 |CC1 | A2 | T1 | R1 | B1 |CH1 | B2 | B3 |JAIL| 10
 *      +----+----+----+----+----+----+----+----+----+----+----+
 *   39 | H2 |                                            | C1 | 11
 *      +----+                                            +----+
 *   38 | T2 |                                            | U1 | 12
 *      +----+                                            +----+
 *   37 | H1 |                                            | C2 | 13
 *      +----+                                            +----+
 *   36 |CH3 |                                            | C3 | 14
 *      +----+                                            +----+
 *   35 | R4 |                                            | R2 | 15
 *      +----+                                            +----+
 *   34 | G3 |                                            | D1 | 16
 *      +----+                                            +----+
 *   33 |CC3 |                                            |CC2 | 17
 *      +----+                                            +----+
 *   32 | G2 |                                            | D2 | 18
 *      +----+                                            +----+
 *   31 | G1 |                                            | D3 | 19
 *      +----+----+----+----+----+----+----+----+----+----+----+
 *   30 |G2J | F3 | U2 | F2 | F1 | R3 | E3 | E2 |CH2 | E1 | FP | 20
 *      +----+----+----+----+----+----+----+----+----+----+----+
 *        30   29   28   27   26   25   24   23   22   21   20
 *
 * In addition, there are two piles of cards, CC (Community Chest) and CH
 * (Chance), with 16 cards in each pile. The piles are shuffled at the
 * beginning of the game. When a player lands on a CC or CH square, a card
 * from the top of the respective pile is drawn, the instruction on it is
 * followed, and the card is put back to the bottom of the pile.
 *
 * A player starts on the GO square and adds the scores on two 6-sided dice to
 * determine the number of squares they advance. In addition, the following
 * rules apply:
 *
 * - if one lands on G2J, he goes to JAIL.
 * - if one rolls three consecutive doubles, the 3rd roll does not advance him
 *   but instead sends him to JAIL.
 * - if one lands on CC (Community Chest), 2/16 cards change their position:
 *   - Advance to GO
 *   - Go to JAIL
 * - if one lands on CH (Chance), 10/16 cards change their position:
 *   - Advance to GO
 *   - Go to JAIL
 *   - Go to C1
 *   - Go to E3
 *   - Go to H2
 *   - Go to R1
 *   - Go to next R (railway company)
 *   - Go to next R
 *   - Go to next U (utility company)
 *   - Go back 3 squares (if the resulting square is CC, a card is NOT drawn
 *     again)
 *
 * By starting at GO and numbering the squares sequentially from 00 to 39, we
 * can concatenate these two-digit numbers to produce strings that correspond
 * with sets of squares.
 *
 * Statistically it can be shown that the three most popular squares, in
 * order, are JAIL (6.24%) = Square 10, E3 (3.18%) = Square 24, and GO (3.09%)
 * = Square 00. So these three most popular squares can be listed with the
 * six-digit modal string: 102400.
 *
 * If, instead of using two 6-sided dice, two 4-sided dice are used, find the
 * six-digit modal string.
 *
 * SOLUTION:
 *
 * Dice rolling is a Markov process, and the probability distribution of
 * landing squares is the equilibrium distribution. However, two rules
 * complicate the analysis:
 *
 * 1. The third consecutive roll of doubles sends the player to JAIL. To model
 *    this, we create 3 super-states which correspond to the number of
 *    consecutive doubles (0, 1, 2) the player has rolled so far. This makes a
 *    total of 120 states and a 120-by-120 transition probability matrix.
 *
 * 2. The CC and CH cards are not drawn randomly, but from top to bottom. To
 *    cope with this, we make a simplification by assuming that the cards were
 *    drawn randomly. Since the cards are shuffled at the beginning, this
 *    simplification should not affect the equilibrium distribution.
 *
 * After we build the transition matrix, we use standard Markov theory to find
 * the stationary distribution. See http://en.wikipedia.org/wiki/Markov_chain
 * for details. A summary is provided below:
 *
 * Let P be the transition probability matrix. Suppose that a unique steady
 * distribution exists, and denote it by the row vector x. In particular, the
 * steady distribution is independent of the starting probabilities (i.e. in
 * which square the player starts). We have
 *
 *   x * P = x
 *
 * which gives us
 *
 *   x * (P - I) = 0
 *
 * This can be transformed to the standard linear system form
 *
 *   (P - I)' * x' = 0
 *
 * However, det(P - I) = 0 and the solution is not unique. To address this we
 * replace one of the equations (e.g. the first equation) by
 *
 *   1 * x' = 1
 *
 * that is, elements of x sum to one. We can then solve the amended system to
 * obtain the steady distribution.
 */

#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
#include "euler/matrix.hpp"
#include "euler.h"

BEGIN_PROBLEM(84, solve_problem_84)
  PROBLEM_TITLE("Most Popular Squares in the Game of Monopoly")
  PROBLEM_ANSWER("101524")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("N^3")
  PROBLEM_SPACE_COMPLEXITY("N^2")
  PROBLEM_KEYWORDS("probability")
END_PROBLEM()

static const char * const squares[40] = {
  "GO",   "A1", "CC1",  "A2", "T1", "R1",  "B1", "CH1", "B2", "B3",
  "JAIL", "C1",  "U1",  "C2", "C3", "R2",  "D1", "CC2", "D2", "D3",
  "FP",   "E1", "CH2",  "E2", "E3", "R3",  "F1",  "F2", "U2", "F3",
  "G2J",  "G1",  "G2", "CC3", "G3", "R4", "CH3",  "H1", "T2", "H2"
};

static double P[120][120]; /* transition matrix */

#define INDEX(doubles,square) ((doubles)*40+(square))

// Finds the next square that begins with _letter_ and returns its location.
static int find_next(char letter, int loc)
{
  while (squares[loc = (loc + 1) % 40][0] != letter);
  return loc;
}

// Fills a row in a transition probability matrix _P_, corresponding to the
// player currently in square _loc_. The number of consecutive doubles rolled
// so far is ignored.
static void fill_transition_matrix_row(double P[40], int loc, double p0)
{
  if (strncmp(squares[loc], "G2J", 3) == 0)
  {
    P[10] += p0; // JAIL
    return;
  }

  if (strncmp(squares[loc], "CH", 2) == 0)
  {
    double p1 = p0 / 16.0;
    P[0]  += p1; // GO
    P[10] += p1; // JAIL
    P[11] += p1; // C1
    P[24] += p1; // E3
    P[39] += p1; // H2
    P[5]  += p1; // R1
    P[find_next('R', loc)] += 2 * p1; // next R
    P[find_next('U', loc)] += p1; // next U
    P[loc-3] += p1; // back 3 squares
    P[loc] += 6 * p1;
    return;
  }

  if (strncmp(squares[loc], "CC", 2) == 0)
  {
    double p1 = p0 / 16.0;
    P[0] += p1; // GO
    P[10] += p1; // JAIL;
    P[loc] += 14 * p1;
    return;
  }

  // No special treatment.
  P[loc] += p0;
}

static void build_transition_matrix(int sides)
{
  const double p0 = 1.0 / (sides * sides); // probability of each roll
  memset(P, 0, sizeof(P));

  for (int doubles = 0; doubles < 3; doubles++)
  {
    for (int square = 0; square < 40; square++)
    {
      int i = INDEX(doubles, square);
      for (int d1 = 1; d1 <= sides; d1++) // dice 1
      {
        for (int d2 = 1; d2 <= sides; d2++) // dice 2
        {
          // Process third consecutive double.
          if (d1 == d2 && doubles == 2)
          {
            int j = INDEX(0, 10); // JAIL
            P[i][j] += p0;
            continue;
          }

          // Find next super-state of doubles.
          int next_doubles = (d1 == d2)? doubles + 1 : 0;

          // Advance the position.
          int next_square = (square + d1 + d2) % 40;

          // Fill in the appropriate row in the transition matrix.
          fill_transition_matrix_row(&P[i][INDEX(next_doubles, 0)],
            next_square, p0);
        }
      }
    }
  }
}

template <class T, size_t N>
static void markov_steady_distribution(
    const T (&transition_matrix)[N][N], T prob[])
{
  // Create a matrix for further processing.
  using Matrix = euler::matrix<T,N,N>;
  Matrix P(transition_matrix);

  // Create a matrix to store A := (P - I)'.
  Matrix A = euler::transpose(P - Matrix::identity());

  // Replace the first row of A by (1,...,1).
  for (size_t j = 0; j < N; j++)
  {
    A[0][j] = T(1);
  }

  // Solve the linear equation A*prob = (1,0,...,0)'.
  euler::matrix<T,N,1> b { 1.0 };
  euler::matrix<T,N,1> x = euler::solve(A, b);
  for (size_t i = 0; i < N; i++)
  {
    prob[i] = x[i][0];
  }
}

static void solve_problem_84()
{
#if 0
  const int sides = 6;
#else
  const int sides = 4;
#endif

  // Build Markov transition probability matrix.
  build_transition_matrix(sides);

  // Compute steady distribution.
  double x[120];
  markov_steady_distribution(P, x);

  // Combine the probability in three sub-states.
  double prob[40] = { 0.0 };
  for (int i = 0; i < 40; i++)
  {
    prob[i] = x[i] + x[40+i] + x[80+i];
  }
  if (verbose())
  {
    for (int i = 0; i < 40; i++)
    {
      std::cout << "Pr(" << i << ") = " << prob[i] << std::endl;
    }
  }

  // Sort the probabilities in descending order.
  int order[40];
  std::iota(order+0, order+40, 0);
  std::partial_sort(order+0, order+3, order+40, [&](int i, int j) -> bool {
    return prob[i] > prob[j];
  });

  // Produce a code from the square index of the top three probabilities.
  char result[7];
  for (int i = 0; i < 3; i++)
  {
    result[2*i] = '0' + order[i] / 10;
    result[2*i+1] = '0' + order[i] % 10;
  }
  result[6] = '\0';

  // Output answer.
  std::cout << result << std::endl;
}
