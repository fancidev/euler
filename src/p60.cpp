/*
 * The primes 3, 7, 109, and 673, are quite remarkable. By taking any two
 * primes and concatenating them in any order the result will always be
 * prime. For example, taking 7 and 109, both 7109 and 1097 are prime.
 * The sum of these four primes, 792, represents the lowest sum for a set
 * of four primes with this property.
 *
 * Find the lowest sum for a set of five primes for which any two primes
 * concatenate to produce another prime.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "euler/prime_test.hpp"
#include "euler.h"

BEGIN_PROBLEM(60, solve_problem_60)
  PROBLEM_TITLE("Set of five primes where any two concatenate to produce another prime")
  PROBLEM_ANSWER("26033")
  PROBLEM_DIFFICULTY(2)
  PROBLEM_FUN_LEVEL(2)
  PROBLEM_TIME_COMPLEXITY("")
  PROBLEM_SPACE_COMPLEXITY("")
  PROBLEM_KEYWORDS("graph,clique")
END_PROBLEM()

// Find the smallest 10^k > n.
template <typename T>
T smallest_10s_power(T n)
{
  T m = 10;
  while (m <= n)
  {
    m *= 10;
  }
  return m;
}

// Represents a graph where the vertices are prime numbers and two vertices
// are connected if their primes concatenate to another prime.
class prime_graph
{
  // Vertices.
  struct vertex
  {
    unsigned int prime; // the prime
    unsigned int power; // the smallest 10^k > prime
    explicit vertex(unsigned int p) :
      prime(p), power(smallest_10s_power(p)) { }
  };
  std::vector<vertex> V;

  // Cache of the edges, i.e. connections between the primes.
  // Only the edges connecting to the newly-generated prime, p are stored,
  // and since primality testing is costly, it is computed on demand.
  struct edge
  {
    bool connected; // whether vertex v is connected to vertex p.
    bool computed;  // whether ``connected'' is computed.
    edge() : connected(false), computed(false) { }
  };
  std::vector<edge> cache;

  bool compute_connection(int i, int j) const
  {
    const vertex &u = V[i], &v = V[j];
    return euler::is_prime(u.prime + v.prime * u.power)
      && euler::is_prime(v.prime + u.prime * v.power);
  }

public:

  unsigned int get_vertex(int i) const { return V[i].prime; }

  size_t size() const { return V.size(); }

  // Adds p to the vertex set.
  // Returns the index of the new vertex.
  int add_vertex(unsigned int p)
  {
    int index = static_cast<int>(V.size());
    V.push_back(vertex(p));
    cache.resize(V.size());
    std::fill(cache.begin(), cache.end(), edge());
    return index;
  }

  // Checks whether V[i] and V[j] are linked.
  bool is_connected(int i, int j)
  {
    if (j == static_cast<int>(V.size()) - 1)
    {
      if (!cache[i].computed)
      {
        cache[i].connected = compute_connection(i, j);
        cache[i].computed = true;
      }
      return cache[i].connected;
    }
    else
    {
      return compute_connection(i, j);
    }
  }
};

// Todo: we might optimize it to store maximal cliques only. Remember to
// remove duplicates.
// Todo: we may reduce the storage of cliques by storing its vertices in a
// dynamic array instead of a static array. However that's not a bottleneck.
// Todo: make use of Bron-Kerbosch algorithm to optimize clique generation.
// Todo: take advantage of the sparsity of the graph to optimize the algorithm.
static void solve_problem_60()
{
  const int K = 5;

  struct clique
  {
    int vertices[K];
    unsigned int weight;

    clique() : weight(0) { }
  };

  prime_graph G;
  std::vector<std::vector<clique>> clique_groups(K+1);

  // Initialize group[0] with an empty clique from which cliques of more
  // vertices may be grown.
  clique_groups[0].push_back(clique());

  unsigned int min_weight = std::numeric_limits<unsigned int>::max();
  int min_clique = -1;

  // Enumerate each prime in turn, starting from 3.
  for (unsigned int p = 3; ; p = euler::next_prime(p))
  {
    // Add p to the vertex set.
    int index = G.add_vertex(p);

    // Grow each k-clique group for k = 0 to K-1.
    bool empty = true; // indicates that all clique groups are empty.
    for (int k = 0; k < K; k++)
    {
      // Traverse each k-clique.
      for (auto it = clique_groups[k].begin(); it != clique_groups[k].end(); ++it)
      {
        const clique &c = *it;

        // Skip cliques just appended in the previous iteration.
        if (k > 0 && c.vertices[k-1] == index)
        {
          break;
        }

        // Remove c from k-clique group if any K-clique grown
        // from c will definitely exceed the upper bound.
        // Todo: do an actual removal instead of check for bounds
        // each time.
        if (c.weight + (K-k)*p < min_weight)
        {
          empty = false;

          // Check whether p is connected to every vertex in c.
          bool connected = std::all_of(&c.vertices[0], &c.vertices[k],
            [&G, index](int i) -> bool {
              return G.is_connected(i, index);
          });
          if (connected)
          {
            clique c2(c);
            c2.weight += p;
            c2.vertices[k] = index;
            clique_groups[k+1].push_back(c2);

            // If new K-cliques are generated, update min_weight.
            if (k+1 == K && c2.weight < min_weight)
            {
              min_weight = c2.weight;
              min_clique = static_cast<int>(clique_groups[K].size()) - 1;
            }
          }
        }
      }
    }

    // If no clique is left available, terminate.
    if (empty)
    {
      break;
    }
  }

  // Print the smallest K-clique.
  std::cout << min_weight << std::endl;

  // Print the smallest clique.
  if (verbose())
  {
    std::cout << "Found clique:";
    const auto &vv = clique_groups[K][min_clique].vertices;
    for (int v: vv)
    {
      std::cout << " " << G.get_vertex(v);
    }
    std::cout << std::endl;
  }

  // Print statistics.
  if (verbose())
  {
    std::cout << "# primes generated: " << G.size() << std::endl;
    for (int k = 1; k <= K; k++)
    {
      std::cout << k << "-Clique Group contains " <<
        clique_groups[k].size() << " cliques." << std::endl;
    }
  }
}
