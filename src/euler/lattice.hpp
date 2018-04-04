#ifndef EULER_LATTICE_H
#define EULER_LATTICE_H

#include <vector>
#include "int_traits.hpp"
#include "gcd.hpp"

namespace euler {

// Enumerates primitive lattice points on the sphere with radius r and
// center at origin. For each lattice point with coordinates (a,b,c),
// calls f(a,b,c).
//
// PARAMETERS
// T:    must be an integer type
// Func: must be a function that takes three parameters of type T.
//       Its return value (if any) is ignored.
// R:    must be greater than or equal to zero.
//
// COMPLEXITY
// Time:
// Stack space: constant
// Heap space:  no more than (2*sizeof(T))*(0.15115*r+1.07736*sqrt(r)+1) bytes
template <class T, class Func>
void sphere_primitive_lattice(T R, Func f)
{
	// Convert T to unsigned to simplify calculations.
	// typedef typename int_traits<T>::unsigned_type T;
	if (R < 0)
		return;
	T r = R;

	// Find coprime (m,n,p,q) such that
	// a = m^2 + n^2 - p^2 - q^2
	// b = 2(mq + np)
	// c = 2(nq - mp)
	// d = m^2 + n^2 + p^2 + q^2
	int count = 0;

	// We start by finding all m^2 + n^2 + p^2 + q^2 = r where
	// m <= n <= p <= q. To improve speed, we build a cache of
	// all x^2 + y^2 = t for t <= d / 2.
	typedef typename int_traits<T>::narrow_type T_sqrt;
	struct cache_entry
	{
		T_sqrt x, y;
		T x2y2;
	public:
		cache_entry(T_sqrt _x, T_sqrt _y, T _x2y2) : x(_x), y(_y), x2y2(_x2y2) { }
	};
	auto comp = [](const cache_entry &e1, const cache_entry &e2) -> bool {
		return (e1.x2y2 < e2.x2y2) || (e1.x2y2 == e2.x2y2 && e1.y > e2.y);
	};
	std::vector<cache_entry> cache;
	cache.reserve((T)(0.15115 * r + 1.07736 * std::sqrt((double)r) + 1));

	// d >= 4*m^2 => m^2 <= d/4
	// d >= m^2 + 3*n^2 => n^2 <= (d-m^2)/3
	// Todo: deal with overflow of x.
	for (T x = 0; x*x <= r/4; x++)
	{
		for (T y = x; y*y <= (r-x*x)/3; y++)
		{
			cache.push_back(	cache_entry(x, y, x*x+y*y));
		}
	}
	std::sort(cache.begin(), cache.end(), comp);

	// We proceed to enumerate all p <= q.
	// r <= 4*q^2 => q >= sqrt(r)/2
	// r <= 3*p^2 + q^2 => p >= sqrt((r-q^2)/3)
	// Todo: remove dependence on double in case T is too big for a double.
	for (T q = (T)(sqrt((double)r)/2.0); q <= (T)std::sqrt((double)r); q++)
	{
		for (T p = (T)sqrt((r-q*q)/3.0); p <= q; p++)
		{
			T p2q2 = p*p + q*q;
			T key = r - p2q2;
			auto it = std::lower_bound(cache.cbegin(), cache.cend(), cache_entry(0,p,key), comp);
			for (; it != cache.cend() && it->x2y2 == key; ++it)
			{
				++count;

				// Try each permutation of (m,n,p,q) to generate (a,b,c,d).
				T m = it->x, n = it->y;
				T mnpq[4] = {m,n,p,q};
				do
				{
					T mm = mnpq[0], nn = mnpq[1], pp = mnpq[2], qq = mnpq[3];
					if (mm*mm + nn*nn >= pp*pp + qq*qq && nn*qq >= mm*pp)
					{
						T a =  mm*mm + nn*nn - pp*pp - qq*qq;
						T b = 2*(mm*qq + nn*pp);
						T c = 2*(nn*qq - mm*pp);
						T d = r; // mm*mm + nn*nn + pp*pp + qq*qq;
						T gcd_ab = gcd(a,b);
						T gcd_cd = (gcd_ab == 1)? 0 : gcd(c,d);
						if ((gcd_ab == 1 || gcd_cd == 1 || gcd(gcd_ab,gcd_cd) == 1)
							&& (pp != 0 || mm <= nn) && (mm != 0 || pp <= qq))
						{
							//std::cout << a << "," << b << "," << c << " ("
							//	<< mm << "," << nn << "," << pp << "," << qq << ")" << std::endl;
							f(a,b,c);
						}
					}
				} while (std::next_permutation(&mnpq[0], &mnpq[4]));
			}
		}
	}
}

#if 0
// Enumerates primitive lattice points on the circle with radius r
// and center at origin.
template <class T, class Func>
void circle_primitive_lattice(T r, Func f)
{
	// Find coprime m > n such that m^2 + n^2 = r.
	for (T n = 1; ; n++)
	{
		T m = (T)std::sqrt((double)(r - n*n));
		if (m <= n)
			break;
		if (m*m == r - n*n && gcd(m,n) == 1)
		{
			T a = m*m - n*n;
			T b = 2 * m * n;
			f(a, b);
		}
	}
}
#endif

} // namespace euler

#endif
