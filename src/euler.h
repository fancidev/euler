#ifndef EULER_H
#define EULER_H

typedef void (*PROBLEM_FUNC) ();

/// @defgroup Problems Problems

/// @defgroup Helper Helper Functions
/// @ingroup Problems

/**
 * Contains information about a solution to a problem.
 *
 * Each solution is identified by the ID of the problem it solves.
 *
 * @ingroup Helper
 */
struct euler_problem_info
{
  int id;                      /**< Problem number */
  PROBLEM_FUNC routine;        /**< Entry point of solution routine */
  const char *title;           /**< Problem title */
  int difficulty;              /**< Problem difficulty (on scale of 1 to 3) */
  int fun_level;               /**< Problem fun level (on scale of 1 to 3) */
  const char *time_complexity; /**< Time complexity of solution */
  const char *space_complexity;/**< Space complexity of solution */
  const char *answer;          /**< Answer to the problem */
  const char *keywords;        /**< Keywords separated by comma */

public:
  /// Constructs an empty object.
  euler_problem_info() : id(0), routine(), title(), difficulty(0),
    fun_level(0), time_complexity(), space_complexity(),
    answer(), keywords() { }
};

/**
 * Registers the solution in the global solution map. The solution is 
 * identified by its problem ID. If a solution with the same ID already
 * exists in the global map, it is substituted with the new one.
 *
 * @param info Information about the solution to register.
 * @ingroup Helper
 */
extern void register_problem(const euler_problem_info &info);

/// Begins the declaration of a solution for a specific problem.
/// @param pid Problem ID.
/// @param _routine Entry point of the solution routine.
/// @ingroup Helper
#define BEGIN_PROBLEM(pid, _routine) \
  static void _routine(); \
  static int _init_problem_##pid(); \
  static int _var_problem_##pid = _init_problem_##pid(); \
  static int _init_problem_##pid() { \
  euler_problem_info info; \
  info.id = pid; \
  info.routine = _routine;

/// Sets the title of the problem.
/// @ingroup Helper
#define PROBLEM_TITLE(x) info.title = x;

/// Sets the answer to the problem.
/// @ingroup Helper
#define PROBLEM_ANSWER(x) info.answer = x;

/// Sets the difficulty of the problem.
/// @ingroup Helper
#define PROBLEM_DIFFICULTY(x) info.difficulty = x;

/// Sets the fun level of the problem.
/// @ingroup Helper
#define PROBLEM_FUN_LEVEL(x) info.fun_level = x;

/// Sets the time complexity of the solution.
/// @ingroup Helper
#define PROBLEM_TIME_COMPLEXITY(x) info.time_complexity = x;

/// Sets the space complexity of the solution.
/// @ingroup Helper
#define PROBLEM_SPACE_COMPLEXITY(x) info.space_complexity = x;

/// Sets the keywords of the problem. Multiple keywords can be separated by
/// comma.
/// @ingroup Helper
#define PROBLEM_KEYWORDS(x) info.keywords = x;

/// Ends the declaration of a solution.
/// @ingroup Helper
#define END_PROBLEM() \
  register_problem(info); \
  return 0; \
  }

#endif
