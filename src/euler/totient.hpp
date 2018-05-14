#ifndef EULER_TOTIENT_H
#define EULER_TOTIENT_H

#include <vector>
#include "prime_factor.hpp"

namespace euler {

template <typename T>
T totient(T n)
{
  T ret = n;
  prime_factorize_distinct(n, [&ret](T p, int k) {
    ret = ret / p * (p - 1);
  });
  return ret;
}

template <typename T>
void totient_table(std::vector<T> &phi)
{
  if (phi.empty())
  {
    return;
  }
  const T N = static_cast<T>(phi.size()) - 1;
  for (T n = 0; n <= N; n++)
  {
    phi[n] = (n % 2 == 0) ? n / 2 : n;
  }
  for (T p = 3; p <= N; p += 2)
  {
    if (phi[p] == p)
    {
      for (T k = p; k <= N; k += p)
      {
        phi[k] = phi[k] / p * (p - 1);
      }
    }
  }
}

} // namespace euler

#endif // EULER_TOTIENT_H
