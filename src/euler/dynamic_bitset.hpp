#ifndef EULER_DYNAMIC_BITSET_HPP
#define EULER_DYNAMIC_BITSET_HPP

#include <vector>

namespace euler {

/**
 * Dynamic bitset.
 *
 * This class implements the equivalent of vector<bool> (or dynamic_bitset)
 * to work around a performance issue of vector<bool> under VC++ 64-bit:
 *
 *   32-bit, vector<bool> : 2.49 s
 *   32-bit, vector<char> : 2.76 s
 *   64-bit, vector<bool> : 3.40 s
 *   64-bit, vector<char> : 2.73 s
 *
 * The performance hit comes from writing to a vector<bool> in 64-bit.
 * The cause might have something to do with the fact that in the STL
 * implementation, the word size used to store the bool vector is fixed
 * at 4 bytes under both 32-bit and 64-bit environment. This might cause
 * a performance hit in the latter case due to alignment issue, etc.
 *
 * In our implementation, we compared the choice of unsigned char with
 * size_t, where the latter corresponds to the native integer size. 
 * The profiling results are listed below.
 * 
 *   64-bit, size_t (8 bytes) : 2.43 s
 *   64-bit, uint8_t (1 byte) : 2.42 s
 *   32-bit, size_t (4 bytes) : 2.47 s
 *   32-bit, uint8_t (1 byte) : 2.40 s
 * 
 * Taking into account performance and simplicity, we choose unsigned char
 * in this implementation.
 */
template <typename TMask = unsigned char>
class dynamic_bitset
{
	static const int BitsPerWord = sizeof(TMask)*8;

	std::vector<TMask> table;

public:
	dynamic_bitset(size_t N, bool value)
		: table((N+BitsPerWord-1)/BitsPerWord, (TMask)(value? -1 : 0))
	{
	}

	bool test(size_t index) const
	{
		return (table[index / BitsPerWord] & ((TMask)1 << (index % BitsPerWord))) != 0;
	}

	bool operator [](size_t index) const { return test(index); }

	void set(size_t index)
	{
		table[index/BitsPerWord] |= ((TMask)1 << (index % BitsPerWord));
	}

	void reset(size_t index)
	{
		table[index/BitsPerWord] &= ~((TMask)1 << (index % BitsPerWord));
	}
};

} // namespace euler

#endif // EULER_DYNAMIC_BITSET_HPP
