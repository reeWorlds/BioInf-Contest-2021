#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
using namespace std;

#define pii pair<int, int>

ifstream in("70-welcome-approx.txt");
ofstream out("output.txt");

// for faster partial solution uncomment next line
//#define FastPartialSolution
// 200 * 0.88842 score on 70-welcome-approx.txt
// 300 * 0.61238 score on 90-huge-approx.txt
// you can also increase constant next in code (line 160) to get much better score

int n, q, dt;
// reference isoforms
vector <vector <pii> > rf;
// complement reference isoforms
vector <vector <pii> > rf2;

// requests isoforms
vector <vector <pii> > inf;
// complement requests isoforms
vector <vector <pii> > inf2;
// lens of requests isoforms and complement isoforms
vector <int> sum1, sum2;

vector <int> res;

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

// returns complement isoform
vector <pii> getIntrons(vector <pii>& dat)
{
	vector <pii> res;

	for (int i = 1; i < dat.size(); i++)
	{
		res.push_back({ dat[i - 1].second + 1, dat[i].first - 1 });
	}

	return res;
}

// returns len of isoform
int getSum(vector <pii>& dat)
{
	int sum = 0;

	for (auto it : dat)
	{
		sum += it.second - it.first + 1;
	}

	return sum;
}

// unites 2 intervals (return len)
inline int subUnite(pii& p1, pii& p2)
{
	return max(0, min(p1.second, p2.second) - max(p2.first, p1.first));
}

// usites 2 isoforms, linear time, 2 iterators
int getUnionSize(vector <pii>& s1, vector <pii>& s2)
{
	int res = 0, i1 = 0, i2 = 0;

	while (i1 < s1.size() && i2 < s2.size())
	{
		res += subUnite(s1[i1], s2[i2]);

		if (s1[i1].second < s2[i2].second)
		{
			i1++;
		}
		else
		{
			i2++;
		}
	}

	return res;
}

int main()
{
	string ts;

	// read input
	in >> n >> dt;
	for (int i = 0; i < n; i++)
	{
		in >> ts;

		vector <pii> parsed = parse(ts);

		rf.push_back(parsed);
		rf2.push_back(getIntrons(parsed));
	}
	in >> q;
	for (int i = 0; i < q; i++)
	{
		in >> ts;

		vector <pii> parsed = parse(ts);

		inf.push_back(parsed);
		inf2.push_back(getIntrons(parsed));

		sum1.push_back(getSum(inf.back()));
		sum2.push_back(getSum(inf2.back()));
	}

	res.resize(inf.size(), 0);

	// combine isoforms with each other
	// use OpenMP
#pragma omp parallel for num_threads(7)
	for (int i = 0; i < inf.size(); i++)
	{
		double bestScore = -1.0;

#ifdef FastPartialSolution
		mt19937 rng(47);
#endif // FastPartialSolution

#ifdef FastPartialSolution
		for (int jj = 0; jj < 2000; jj++)
#else
		for (int j = 0; j < rf.size(); j++)
#endif // FastPartialSolution
		{

#ifdef FastPartialSolution
			int j = rng() % rf.size();
#endif // FastPartialSolution

			double myScore = 0;

			int s1 = getUnionSize(inf[i], rf[j]);
			int s2 = getUnionSize(inf2[i], rf2[j]);

			int d1 = sum1[i];
			int d2 = sum2[i];

			myScore = 2.0 * s1 / d1 + 1.0 * s2 / d2;

			if (myScore > bestScore)
			{
				bestScore = myScore;
				res[i] = j;
			}
		}
	}

	// output result
	for (int i = 0; i < inf.size(); i++)
	{
		out << res[i] << "\n";
	}

	return 0;
}