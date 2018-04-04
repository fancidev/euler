#ifndef EULER_COMBINATION_H
#define EULER_COMBINATION_H

#include <vector>
#include <cassert>

namespace euler {

template <typename TIndex>
class combination_generator
{
	TIndex n; // total number of elements
	TIndex r; // number of elements to choose
	std::vector<TIndex> choice;

public:

	combination_generator(TIndex from, TIndex choose)
		: n(from), r(choose), choice(r)
	{
		assert(n >= r && r >= 0);
		for (TIndex i = 0; i < r; i++)
			choice[i] = i;
	}

	size_t size() const { return n; }

	TIndex operator [] (int i) const { return choice[i]; }

	bool next()
	{
		if (r == 0)
			return false;
		if (choice[0] == n - r)
			return false;

		TIndex i = r - 1;
		while (choice[i] == n - (r - i))
			--i;

		++choice[i];
		for (TIndex j = i + 1; j < r; j++)
			choice[j] = choice[i] + (j - i);
		return true;
	}

};


} // namespace euler

#endif // EULER_COMBINATION_H
