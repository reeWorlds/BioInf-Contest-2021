#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

#define pii pair<int, int>

// for reading input
ifstream in("test3.txt");
// for best superspreader in each testcase
ofstream outMain("output.txt");
// for top10 superspreaders in each testcase
ofstream out("info.txt");

class Sol
{
	int n, d;
	
	// edges in communication graph
	// edges[i].first.first - human that can infect other one
	// edges[i].first.second - himan that can be infected by other one
	// corresponding probability
	vector <vector <pair<pii, double> > > edges; // { {spreader, reciever}, probability}

public:

	void solve(int testCase)
	{
		in >> n >> d;

		// read input
		for (int i = 0; i < d; i++)
		{
			vector <pair<pii, double> > tedges;

			int tn;

			in >> tn;

			for (int j = 0; j < tn; j++)
			{
				int a, b;
				double c;

				in >> a >> b >> c;
				a--;
				b--;

				tedges.push_back({ {a, b}, c });
			}

			edges.push_back(tedges);
		}

		// find all humans that can infect someone on day 1
		vector <int> bestCands;
		vector <double> bestCandVal;

		set <int> tmp;
		for (auto it : edges[0])
		{
			tmp.insert(it.first.first);
		}

		for (auto it : tmp)
		{
			bestCands.push_back(it);
		}

		bestCandVal.resize(bestCands.size(), 0.0);

		cout << "Total cands = " << bestCands.size() << "\n";

		// test each human as superspreader
#pragma omp parallel for num_threads(6)
		for (int i = 0; i < bestCands.size(); i++)
		{
			// v - starting spreader
			int v = bestCands[i];

			// inf - currect probabilities
			// nxtInf - probabilities on the next day
			vector <double> inf(n, 0), nxtInf(n, 0);

			// set initial spreader
			inf[v] = nxtInf[v] = 1.0;

			// model virus spreading for each day
			for (auto& day : edges)
			{
				// update probabilities
				for (auto& e : day)
				{
					nxtInf[e.first.second] = nxtInf[e.first.second] +
						(1.0 - nxtInf[e.first.second]) * inf[e.first.first] * e.second;
				}

				// set next day probabilities as current day
				for (auto& e : day)
				{
					inf[e.first.second] = nxtInf[e.first.second];
				}
			}

			// calculate average number of infected humans
			double sum = 0;
			for (auto it : inf)
			{
				sum += it;
			}

			bestCandVal[i] = sum;
		}

		// sort all candidate superspreaders by average number of infected pepople
		vector <pair<double, int> > inf;

		for (int i = 0; i < bestCandVal.size(); i++)
		{
			inf.push_back({ bestCandVal[i], bestCands[i] });
		}

		sort(inf.begin(), inf.end());
		reverse(inf.begin(), inf.end());

		// write best superspreader
		outMain << inf[0].second + 1 << "\n";
		outMain.flush();

		// write top10 superspreaders
		out << "Case = " << testCase << "\n";
		for (int i = 0; i < 10; i++)
		{
			out << "{ " << inf[i].second + 1 << " " << inf[i].first << " }\n";
		}
	}
};


int main()
{
	int n, i;

	in >> n;

	for (i = 0; i < n; i++)
	{
		// process each testcase separately
		Sol s;

		s.solve(i);
	}

	return 0;
}