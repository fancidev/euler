#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <map>
#include "euler.h"

typedef std::pair<int,euler_problem_info> problem_entry_t;

static std::map<int,euler_problem_info>& problems()
{
	static std::map<int,euler_problem_info> p;
	return p;
}

void register_problem(const euler_problem_info &info)
{
	problems().insert(problem_entry_t(info.id, info));
}

static void usage()
{
	std::cout << "usage: euler [options] id ..." << std::endl;
	std::cout << "where: id is the problem number to solve." << std::endl;
	std::cout << "options:" << std::endl;
	std::cout << "    -h    display this help screen" << std::endl;
	std::cout << "    -l    list available solutions" << std::endl;
#ifdef _MSC_VER
	std::cout << "    -p    pause before exit" << std::endl;
#endif
	std::cout << "    -r    run regression test" << std::endl;
	std::cout << "    -s    display statistics" << std::endl;
	std::cout << "    -t    time the calculation" << std::endl;
	std::cout << "    -v    display detailed information" << std::endl;
}

static void list_problems()
{
	std::for_each(problems().cbegin(), problems().cend(), [](const problem_entry_t &ent) {
		const euler_problem_info &p = ent.second;
		std::cout.width(4);
		std::cout << p.id << " " << p.title << std::endl;
	});
}

static PROBLEM_FUNC find_problem(int number)
{
	auto it = problems().find(number);
	if (it == problems().end())
		return NULL;
	else
		return it->second.routine;
}

static bool run_regression(int id, bool timing)
{
	//typedef std::chrono::high_resolution_clock clock;

	if (id == 0)
	{
		std::cout << "  ID STATUS"  << (timing? "     TIME" : "") << std::endl;
		std::cout << "------------" << (timing? "-----------" : "") << std::endl;
	}

	double total_time = 0;
	int nfailed = 0;
	for (auto it = problems().cbegin(); it != problems().cend(); ++it)
	{
		const euler_problem_info &p = it->second;
		if ((id != 0 && p.id != id) || p.routine == NULL)
			continue;

		// Print problem id.
		std::cout << std::setw(4) << p.id << " ";
		std::cout.flush();

		// Redirect stdout buffer to a string.
		std::ostringstream ss;
		std::streambuf *oldbuf = std::cout.rdbuf(ss.rdbuf());

		// Execute and time the routine. The output is stored in ss.
		//clock::time_point start = clock::now();
		p.routine();
		//std::chrono::duration<double> t1 = clock::now() - start;
		//total_time += t1.count();

		// Restore stdout.
		std::cout.rdbuf(oldbuf);

		// Get result from the buffer. Only retain the first line.
		std::string result = ss.str();
		auto k = result.find_first_of('\n');
		if (k != result.npos)
			result.resize(k);

		// Check routine result.
		bool ok = (p.answer != NULL && result == p.answer);

		// Print test status.
		std::cout << (ok? "OK    " : "FAILED");
#if 0
		if (timing)
		{
			std::cout << " [" << std::setw(7) << std::setprecision(3) 
				<< std::fixed << t1.count() << "]";
		}
#endif
		std::cout << std::endl;

		if (!ok)
			++nfailed;
	}
	
	// Display regression test summary.
	if (id == 0)
	{
		std::cout << "------------" << (timing? "-----------" : "") << std::endl;
		if (nfailed == 0)
			std::cout << " ALL";
		else
			std::cout << std::setw(4) << nfailed;
		std::cout << ' ' << (nfailed > 0? "FAILED" : "OK    ");
		if (timing)
		{
			std::cout << " [" << std::setw(7) << std::setprecision(3) 
				<< std::fixed << total_time << "]";
		}
		std::cout << std::endl;
	}
	return (nfailed == 0);
}

static void run_solution(int id)
{
	PROBLEM_FUNC f = find_problem(id);
	f();
}

static void display_statistics()
{
	std::cout << "Number of solutions: " << problems().size() << std::endl;
	if (problems().size() > 0)
	{
		std::cout << "Progress:" << std::endl;
		std::cout << "---------+----------+----------+----------+----------+----------+" << std::endl;
		int max_id = problems().crbegin()->first;
		for (int i = 1; i <= max_id; i += 50)
		{
			std::cout << ' ';
			std::cout.width(3);
			std::cout << i << "-";
			std::cout.width(3);
			std::cout << (i+49) << " |";
			for (int j = 0; j <= 49; j++)
			{
				char c = find_problem(i+j)? '*' : ' ';
				std::cout << c;
				if (j < 49 && j % 10 == 9)
					std::cout << ' ';
			}
			std::cout << "|" << std::endl;
		}
		std::cout << "---------+----------+----------+----------+----------+----------+" << std::endl;
	}
}

int main(int argc, char *argv[])
{
	enum {
		action_default,
		action_regression,
		action_stat,
		action_list,
		action_help
	} action = action_default;

	bool verbose = false;
	bool timing = false;
#ifdef _MSC_VER
	bool pause = false;
#endif
	int id = 0;
	bool test_ok = true;
	(void)verbose;

	// Parse command line arguments.
	for (int i = 1; i < argc; i++)
	{
		const char *s = argv[i];
		if (s[0] == '-')
		{
			for (int j = 1; s[j] != '\0'; j++)
			{
				switch (s[j])
				{
				case 'h':
					action = action_help;
					break;
				case 'l':
					action = action_list;
					break;
#ifdef _MSC_VER
				case 'p':
					pause = true;
					break;
#endif
				case 'r':
					action = action_regression;
					break;
				case 's':
					action = action_stat;
					break;
				case 't':
					timing = true;
					break;
				case 'v':
					verbose = true;
					break;
				default:
					std::cerr << "Unknown option: -" << s[j] << std::endl;
					return 2;
				}
			}
		}
		else
		{
			id = atoi(s);
			if (!find_problem(id))
			{
				std::cerr << "Cannot find problem #" << id << "." << std::endl;
				return 3;
			}
		}
	}

	switch (action)
	{
	case action_help:
		usage();
		break;
	case action_list:
		list_problems();
		break;
	case action_stat:
		display_statistics();
		break;
	case action_regression:
		test_ok = run_regression(id, timing);
		break;
	default:
		if (id == 0)
		{
			usage();
			return 2;
		}
		run_solution(id);
		break;
	}

#ifdef _MSC_VER
	if (pause)
		system("PAUSE");
#endif
	return (test_ok)? 0 : 1;
}
