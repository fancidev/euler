#ifndef EULER_TOTIENT_H
#define EULER_TOTIENT_H

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

} // namespace euler

#endif // EULER_TOTIENT_H
