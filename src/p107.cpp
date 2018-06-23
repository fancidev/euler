/**
 * Given an undirected graph with positive weights on its edges, find the
 * minimum spanning tree of the graph. Output the difference between the
 * total weight of the graph and the total weight of the minimum spanning
 * tree.
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include "euler.h"

BEGIN_PROBLEM(107, solve_problem_107)
  PROBLEM_TITLE("Minimal network")
  PROBLEM_ANSWER("259679")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(1)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("graph,minimum spanning tree")
END_PROBLEM()

/**
 * Data structure that tracks a set of items numbered 0..(n-1) partitioned
 * into a number of disjoint (non-overlapping) subsets.
 */
class disjoint_set
{
  struct subset
  {
    int next; // id of next item in the subset; -1 if none
    int head; // id of representitive item in the set
  };
  std::vector<subset> _list;
  size_t _num_sets;

public:

  explicit disjoint_set(size_t num_items)
    : _list(num_items), _num_sets(num_items)
  {
    for (size_t i = 0; i < num_items; i++)
    {
      _list[i].next = -1;
      _list[i].head = static_cast<int>(i);
    }
  }

  int find_set(int item) const
  {
    return _list[item].head;
  }

  // Union two items if they're not union yet. Returns true if unioned.
  bool find_union(int item1, int item2)
  {
    int h1 = find_set(item1);
    int h2 = find_set(item2);
    if (h1 == h2)
    {
      return false;
    }

    // find the last element in h1
    int u = h1;
    while (_list[u].next >= 0)
    {
      u = _list[u].next;
    }

    // append h2 to u and update the head in each of h2
    _list[u].next = h2;
    u = h2;
    do
    {
      _list[u].head = h1;
    }
    while ((u = _list[u].next) >= 0);

    --_num_sets;
    return true;
  }

  size_t count_sets() const
  {
    return _num_sets;
  }
};

struct edge
{
  int vertex1;
  int vertex2;
  int weight;
};

template <size_t N>
static std::vector<edge> read_graph(const int (&edge_weights)[N][N])
{
  std::vector<edge> edges;
  for (size_t i = 0; i < N; i++)
  {
    for (size_t j = i; j < N; j++)
    {
      if (edge_weights[i][j] > 0)
      {
        edge e;
        e.vertex1 = static_cast<int>(i);
        e.vertex2 = static_cast<int>(j);
        e.weight = edge_weights[i][j];
        edges.push_back(e);
      }
    }
  }
  return edges;
}

static void solve_problem_107()
{
#if 0
  const int N = 7;
  const int edge_weights[N][N] = {
    {0,16,12,21,0,0,0},
    {16,0,0,17,20,0,0},
    {12,0,0,28,0,31,0},
    {21,17,28,0,18,19,23},
    {0,20,0,18,0,0,11},
    {0,0,31,19,0,0,27},
    {0,0,0,23,11,27,0},
  };
#else
  const int N = 40;
  const int edge_weights[N][N] = {
#include "p107-network.txt"
  };
#endif

  // Convert adjacency list to edge list
  std::vector<edge> edges = read_graph(edge_weights);

  // Compute total weight of the graph
  int total_weight = 0;
  for (const edge &e: edges)
  {
    total_weight += e.weight;
  }

  // Use Kruskal algorithm to find the minimum spanning tree
  std::sort(edges.begin(), edges.end(),
    [](const edge &e1, const edge &e2) -> bool
    {
      return e1.weight < e2.weight;
    });
  disjoint_set dset(N);

  // Add each minimum weight edge into the graph if it doesn't create a cycle
  int min_weight = 0;
  for (const edge &e: edges)
  {
    if (dset.find_union(e.vertex1, e.vertex2))
    {
      min_weight += e.weight;
    }
    if (dset.count_sets() == 1)
    {
      break;
    }
  }

  // Display result
  if (verbose())
  {
    std::cout << "Total weight: " << total_weight << std::endl;
    std::cout << "Minimum spanning tree weight: " << min_weight << std::endl;
  }
  std::cout << (total_weight - min_weight) << std::endl;
}
