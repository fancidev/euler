#ifndef EULER_COPRIME_H
#define EULER_COPRIME_H

#include <utility>
#include <deque>

template <class Func>
void generate_coprime_pairs(Func f)
{
	if (!f(1,1) || !f(2,1) || !f(3,1))
		return;

	std::deque<std::pair<int,int>> list;
	list.push_back(std::pair<int,int>(2, 1));
	list.push_back(std::pair<int,int>(3, 1));
	while (!list.empty())
	{
		std::pair<int,int> mn = list.front();
		list.pop_front();

		int m = mn.first;
		int n = mn.second;

		if (1)
		{
			int mm = 2*m - n;
			int nn = m;
			if (f(mm, nn))
				list.push_back(std::pair<int,int>(mm, nn));
		}
		if (2)
		{
			int mm = 2*m + n;
			int nn = m;
			if (f(mm, nn))
				list.push_back(std::pair<int,int>(mm, nn));
		}
		if (3)
		{
			int mm = m + 2*n;
			int nn = n;
			if (f(mm, nn))
				list.push_back(std::pair<int,int>(mm, nn));
		}
	}
}

#endif // EULER_COPRIME_H
