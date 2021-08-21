#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#define pii pair<int, int>

ifstream in("20-mouse-simple-exact.txt");
ofstream out("output.txt");


int n, q, dt;
// reference isoforms
vector <vector <pii> > rf;
// requests isoforms
vector <vector <pii> > inf;


vector <pii> res;

// parse input from string to vector of pair of integers
vector <pii> parse(string ts)
{
	vector <int> nums;
	int lastL = 0;

	// split numbers
	for (int i = 0; i < ts.size(); i++)
	{
		if (ts[i] == '-' || ts[i] == ',')
		{
			nums.push_back(stoi(ts.substr(lastL, i - lastL + 1)));
			lastL = i + 1;
		}
	}
	nums.push_back(stoi(ts.substr(lastL, ts.size() - lastL)));

	vector <pii> res;

	// group numbers in pairs
	for (int i = 0; i < nums.size(); i += 2)
	{
		res.push_back({ nums[i], nums[i + 1] });
	}

	return res;
}

// cheks if isoform Base (reference) contains isoform what (request)
// linear, 2 iterators
// a lot of edge cases/features, implementation is very messy
int contains(vector <pii>& base, vector <pii>& what)
{
	if (what.size() == 1)
	{
		for (auto& it : base)
		{
			if (it.first - dt <= what[0].first && it.second + dt >= what[0].second)
			{
				return 1;
			}
		}

		return 0;
	}

	int posB = 0, posW = 0;

	while (posB < base.size() && base[posB].second + dt < what[0].first)
	{
		posB++;
	}

	if (posB - 1 + what.size() >= base.size() || base[posB].first > what[0].second + dt)
	{
		return 0;
	}

	{
		pii p1 = base[posB], p2 = what[0];

		if (p1.first - p2.first > dt || abs(p2.second - p1.second) > dt)
		{
			return 0;
		}
	}

	for (int i = 1; i + 1 < what.size(); i++)
	{
		pii p1 = base[posB + i], p2 = what[i];

		if (abs(p1.first - p2.first) > dt || abs(p1.second - p2.second) > dt)
		{
			return 0;
		}
	}

	{
		pii p1 = base[posB - 1 + what.size()], p2 = what.back();

		if (abs(p1.first - p2.first) > dt || p2.second - p1.second > dt)
		{
			return 0;
		}
	}

	return 1;
}

int main()
{
	string ts;

	// read data
	in >> n >> dt;
	for (int i = 0; i < n; i++)
	{
		in >> ts;

		vector <pii> parsed = parse(ts);

		rf.push_back(parsed);
	}
	in >> q;
	for (int i = 0; i < q; i++)
	{
		in >> ts;

		vector <pii> parsed = parse(ts);

		inf.push_back(parsed);
	}

	res.resize(inf.size(), { -1, 0});

	// combine isoforms with each other
	// use OpenMP
#pragma omp parallel for num_threads(8)
	for (int i = 0; i < inf.size(); i++)
	{
		vector <int> mySols;

		for (int j = 0; j < rf.size(); j++)
		{
			if (contains(rf[j], inf[i]))
			{
				mySols.push_back(j);
			}
		}

		if (mySols.size() > 0)
		{
			res[i] = { mySols[0], mySols.size() };
		}
		else
		{
			res[i] = { -1, 0 };
		}
	}

	// output result
	for (int i = 0; i < inf.size(); i++)
	{
		out << res[i].first << " " << res[i].second << "\n";
	}

	return 0;
}