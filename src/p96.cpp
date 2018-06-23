// See notes/p96.tex for details about the algorithm.

#include <bitset>
#include <cassert>
#include <iostream>
#include <string>
#include "euler.h"

BEGIN_PROBLEM(96, solve_problem_96)
  PROBLEM_TITLE("Devise an algorithm to solve Sudoku puzzles")
  PROBLEM_ANSWER("24702")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(3)
  PROBLEM_TIME_COMPLEXITY("N^(N^2+5)")
  PROBLEM_SPACE_COMPLEXITY("N^4")
  PROBLEM_KEYWORDS("puzzle,search")
END_PROBLEM()

#define IJ2K(I,J) ((I)/3*3+(J)/3)
#define KL2I(K,L) ((K)/3*3+(L)/3)
#define KL2J(K,L) ((K)%3*3+(L)%3)

template <size_t N>
size_t lowest_bit_set(const std::bitset<N> &mask)
{
  std::bitset<N> b(mask);
  for (size_t i = 0; i < N; i++)
  {
    if (b.test(i))
    {
      return i;
    }
  }
  return N;
}

struct sudoku_layout
{
  int number[9][9];
};

struct sudoku_state
{
  struct block_info
  {
    std::bitset<9> free_digits;
    block_info() : free_digits(std::bitset<9>().set()) { }
  } row[9], col[9], blk[9];

  std::bitset<81> cell_tainted; // whether a given cell needs to be reduced
  std::bitset<81> cell_free;    // whether a given cell is free

public:

  explicit sudoku_state(const sudoku_layout &L)
    : cell_tainted(std::bitset<81>().set()),
      cell_free(std::bitset<81>().set())
  {
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        if (L.number[i][j])
        {
          fix(i, j, L.number[i][j]);
        }
      }
    }
  }

  // Fix a cell with a number.
  void fix(int i, int j, int number)
  {
    assert(number > 0);
    row[i].free_digits[number-1] = false;
    col[j].free_digits[number-1] = false;
    blk[IJ2K(i,j)].free_digits[number-1] = false;
    cell_free[i*9+j] = false;
    cell_tainted[i*9+j] = false;
  }

  std::bitset<9> candidates(int i, int j) const
  {
    return row[i].free_digits & col[j].free_digits & blk[IJ2K(i,j)].free_digits;
  }

  bool free(int i, int j) const { return cell_free[i*9+j]; }

  bool tainted(int i, int j) const { return cell_tainted[i*9+j]; }

  void taint(int i, int j) { cell_tainted[i*9+j] = true; }

  void untaint(int i, int j) { cell_tainted[i*9+j] = false; }

  // Mark all free cells in the given row / column / block as tainted.
  // This could be optimized.
  void taint_range(int i0, int j0, int k0)
  {
    for (int j = 0; j < 9; j++)
    {
      taint(i0, j);
    }
    for (int i = 0; i < 9; i++)
    {
      taint(i, j0);
    }
    for (int l = 0; l < 9; l++)
    {
      taint(KL2I(k0,l), KL2J(k0,l));
    }
    cell_tainted &= cell_free;
  }

};

// A few optimizations are possible:
// - iterating set bits can be faster by bit tricks.
// - testing for single bit set can be faster by using bit tricks.
static bool solve_recursive(sudoku_layout &L, sudoku_state &S)
{
  // Step 1 [Reduction]. Computes the remaining number of candidates for
  // each tainted cell.
  // - If any cell becomes invalid, returns failure.
  // - If a cell has only 1 candidate left, fix that cell and mark the
  //   cells in its row / column / block as tainted.
  // Repeat this process until no cell is tainted.
  while (S.cell_tainted.any())
  {
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        if (S.tainted(i, j))
        {
          S.untaint(i, j);
          std::bitset<9> cand = S.candidates(i, j);
          size_t nc = cand.count();
          if (nc == 0)
          {
            return false;
          }
          if (nc == 1)
          {
            int n = (int)lowest_bit_set(cand) + 1;
            L.number[i][j] = n;
            S.fix(i, j, n);
            S.taint_range(i, j, IJ2K(i,j));
          }
        }
      }
    }
  }

  // Step 2 [Selection]. Choose the free cell with the fewest candidates.
  // - If no free cell exists, return success.
  // - If two or more free cells have the same number of candidates,
  //   choose the one that will impact the most number of cells directly.
  // - If still have multiple choice, pick the first one in some order.
  int choice_i = -1, choice_j = -1;
  size_t choice_nc = 10, choice_nb = 0;
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      if (!S.free(i, j)) // already fixed
      {
        continue;
      }

      std::bitset<9> candidates = S.candidates(i, j);
      size_t nc = candidates.count();
      if (nc <= choice_nc)
      {
        // Note: to simplify implementation, we actually don't
        // compute the number of free cells impacted by this cell.
        // This works fine enough for this problem, but certainly
        // could be improved.

        // unsigned char nb = S.row_nblank[i] + s.col_nblank[j]
        //   + s.blk_nblank[IJ2K(i,j)] - 3;
        size_t nb = 0;
        if (nc < choice_nc || nb > choice_nb)
        {
          choice_i = i;
          choice_j = j;
          choice_nc = nc;
          choice_nb = nb;
        }
      }
    }
  }
  if (choice_i < 0)
  {
    return true;
  }

  // Step 3 [Trial]. For each candidate in the chosen cell, assume it is
  // the answer. Fix it, and mark its row / column / block as tainted.
  // Then call f(P') where P' is the updated layout.
  // - If f(P') returns success, return success.
  // - Otherwise, try the next candidate in the chosen cell.
  // - If all candidates in the chosen cell return failure, return failure.
  std::bitset<9> cand = S.candidates(choice_i, choice_j);
  for (int n = 0; n < 9; n++)
  {
    if (cand.test(n))
    {
      sudoku_state S1(S);
      S1.fix(choice_i, choice_j, n + 1);
      S1.taint_range(choice_i, choice_j, IJ2K(choice_i, choice_j));
      L.number[choice_i][choice_j] = n + 1;
      if (solve_recursive(L, S1))
      {
        return true;
      }
    }
  }

  return false;
}

static bool solve(sudoku_layout &L)
{
  sudoku_state S(L);
  return solve_recursive(L, S);
}

static void write_latex(const sudoku_layout &s)
{
  std::cout << "\\begin{tabular}{| c c c | c c c | c c c |}" << std::endl;
  std::cout << "\\hline" << std::endl;
  for (int i = 0; i < 9; i++)
  {
    for (int j = 0; j < 9; j++)
    {
      int c = s.number[i][j];
      std::cout << (c<0? 'X' : c==0? ' ' : (char)(c+'0'))
                << (j==8? " \\\\" : " & ");
    }
    std::cout << std::endl;
    if (i % 3 == 2)
    {
      std::cout << "\\hline" << std::endl;
    }
  }
  std::cout << "\\end{tabular}" << std::endl;
}

static void solve_problem_96()
{
  const sudoku_layout layouts[50] = {
#include "p96-sudoku.txt"
  };

  int total = 0;

  for (int id = 0; id < 50; id++)
  {
    sudoku_layout L = layouts[id];
    if (verbose())
    {
      write_latex(L);
    }
    if (solve(L))
    {
      int n = L.number[0][0] * 100 + L.number[0][1] * 10 + L.number[0][2];
      if (verbose())
      {
        std::cout << id << ": " << n << std::endl;
      }
      total += n;
    }
    else
    {
      std::cout << id << ": FAILED" << std::endl;
    }
  }
  std::cout << total << std::endl;
}
