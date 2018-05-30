/**
 * @defgroup partition Partition Function
 *
 * Routines to count the number of partitions.
 *
 * @ingroup Library
 */

#ifndef EULER_PARTITION_HPP
#define EULER_PARTITION_HPP

#include <cassert>
#include <cstddef>
#include <vector>

namespace euler {

/**
 * Functor that computes the number of ways of writing an integer as a sum of
 * positive integers, ignoring order.
 *
 * @tparam T Type of return value. It should be an integral type if the exact
 *    result is required, or a modulo type if just the residue is required.
 *
 * @see http://mathworld.wolfram.com/PartitionFunctionP.html
 *
 * @ingroup partition
 */
template <class T>
class partition_function
{
  std::vector<T> _cache;

  T _compute(size_t n)
  {
    assert(n == _cache.size());
    T p = 0, s = 1;
    for (size_t k = 1; k <= n; k++)
    {
      size_t n1 = k*(3*k-1)/2;
      size_t n2 = k*(3*k+1)/2;
      if (n1 <= n)
      {
        p += s * _cache[n - n1];
      }
      if (n2 <= n)
      {
        p += s * _cache[n - n2];
      }
      if (n1 > n /* && n2 > n */)
      {
        break;
      }
      s *= -1;
    }
    return p;
  }

public:

  partition_function() : _cache(2)
  {
    _cache[0] = 1;
    _cache[1] = 1;
  }

  /**
   * Computes the partition function for argument @c n.
   *
   * @param n Nonnegative integer.
   *
   * @returns If <c>n > 0</c>, the number of ways of writing @c n as a sum of
   *    positive integers, ignoring order. If <c>n == 0</c>, returns @c 1.
   *
   * @algorithm Recurrence relation by Euler.
   *
   * @timecomplexity <c>O(n^1.5)</c> to compute all answers up to @c n.
   *
   * @spacecomplexity <c>O(n)</c>.
   */
  T operator()(size_t n)
  {
    if (n >= _cache.size())
    {
      for (size_t m = _cache.size(); m <= n; m++)
      {
        _cache.push_back(_compute(m));
      }
    }
    return _cache[n];
  }
};

} // namespace euler

#endif // EULER_PARTITION_HPP
