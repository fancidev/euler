#ifndef EULER_TUPLE_H
#define EULER_TUPLE_H

#include <vector>

namespace euler {

struct tuple_simple_visitor
{
	void reset(size_t index) { }
	void increment(size_t index) { }
	void decrement(size_t index) { }
};

template <class TVisitor=tuple_simple_visitor, class TIndex=int>
class tuple_lexico_generator
{
	struct element_t
	{
		TIndex max;
		TIndex current;
		element_t(TIndex _max) : max(_max), current(0) { }
	};
	std::vector<element_t> tuple;

	TVisitor &visitor;

public:

	tuple_lexico_generator(size_t count, TVisitor &_visitor=TVisitor()) 
		: tuple(count, 0), visitor(_visitor)
	{
	}

	tuple_lexico_generator(size_t count, TIndex max, TVisitor &_visitor=TVisitor())
		: tuple(count, max), visitor(_visitor) { }

	template <class FwdIt>
	tuple_lexico_generator(FwdIt max_first, FwdIt max_last, TVisitor &_visitor=TVisitor()) 
		: tuple(max_first, max_last), visitor(_visitor) { }

	TIndex& max(size_t index) { return tuple[index].max; }

	size_t size() const { return tuple.size(); }

	TIndex operator [] (size_t index) const { return tuple[index].current; }

	bool next()
	{
		size_t j = 0;
		for (; j < tuple.size() && tuple[j].current == tuple[j].max - 1; j++)
		{
			tuple[j].current = 0;
			visitor.reset(j);
		}
		if (j >= tuple.size())
		{
			return false;
		}
		else
		{
			++tuple[j].current;
			visitor.increment(j);
			return true;
		}
	}

};

template <class TVisitor=tuple_simple_visitor, class TIndex=int>
class tuple_gray_generator
{
	struct element_t
	{
		TIndex max;     // max value + 1
		TIndex current; // current value
		bool dec;       // decreasing?
		element_t(TIndex _max) : max(_max), current(0), dec(false) { }
	};
	std::vector<element_t> tuple;

	std::vector<int> flags;

	TVisitor visitor;

public:
	
	tuple_gray_generator(size_t count, TIndex max, TVisitor _visitor=TVisitor())
		: tuple(count, max), flags(count+1), visitor(_visitor) 
	{ 
		for (size_t i = 0; i <= count; i++)
			flags[i] = (int)i;
	}

	template <class FwdIt>
	tuple_gray_generator(FwdIt max_first, FwdIt max_last, TVisitor _visitor=TVisitor()) 
		: tuple(max_first, max_last), flags(tuple.size()+1), visitor(_visitor) 
	{ 
		for (size_t i = 0; i <= count; i++)
			flags[i] = (int)i;
	}

	size_t size() const { return tuple.size(); }

	TIndex operator [] (size_t index) const { return tuple[index].current; }

	bool next()
	{
		size_t j = flags[0];
		flags[0] = 0;
		if (j == size()) // finish
		{
			return false;
		}
		else
		{
			element_t &e = tuple[j];
			if (e.dec)
			{
				visitor.decrement(j);
				if (--e.exp == 0)
				{
					e.dec = false;
					flags[j] = flags[j+1];
					flags[j+1] = j+1;
				}
			}
			else
			{
				visitor.increment(j);
				if (++e.exp == e.max - 1)
				{
					e.dec = true;
					flags[j] = flags[j+1];
					flags[j+1] = j+1;
				}
			}
			return true;
		}
	}
};

#if 0
// Below is an example of a visitor for the tuple generator.
// Such visitor is also useful in divisor iterator.
class number_visitor
{
	std::vector<int> digits;
	int d;
	std::vector<size_t> places;

public:
	number_visitor(int ndigits, int _d) : digits(ndigits, _d), d(_d) { }

	template <class FwdIt>
	void set_free_places(FwdIt first, FwdIt last)
	{
		places.assign(first, last);
	}

	void reset() 
	{
		std::fill(digits.begin(), digits.end(), d);
	}

	void reset(size_t index)
	{
		digits[places[index]] = (d == 0)? 1 : 0;
	}

	void increment(size_t index)
	{
		if (++digits[places[index]] == d)
			++digits[places[index]];
	}

	void decrement(size_t index)
	{
		if (--digits[places[index]] == d)
			--digits[places[index]];
	}

	long long value() const 
	{
		return euler::from_digits<10,long long>(digits.begin(), digits.end());
	}
};
#endif

} // namespace euler

#endif // EULER_TUPLE_H
