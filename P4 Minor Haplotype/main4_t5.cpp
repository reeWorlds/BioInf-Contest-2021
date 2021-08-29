#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
using namespace std;

ifstream in("in5.txt");
ofstream out("output.txt");

int S;
double thBeta;
char type;

class Sol
{

public:

	// input data, only s0 and dat are necessary
	string s0;
	int k, n;
	double minRate, alpha;

	vector <string > dat;


	void solve(int testCase)
	{
		string s;

		// read input
		in >> s;
		in >> s0;
		in >> k >> n;
		in >> minRate >> alpha;

		for (int i = 0; i < n; i++)
		{
			in >> s;

			dat.push_back(s);
		}

		// find all genomes that differ from s0 by at least 15 sites
		vector <string > h2;

		for (auto it : dat)
		{
			int cntErr = 0;

			for (int i = 0; i < s0.size(); i++)
			{
				cntErr += s0[i] != it[i];
			}

			if (cntErr > 15)
			{
				h2.push_back(it);
			}
		}

		if (h2.size() == 0)
		{
			out << 1 << "\n";
		}
		else
		{
			out << 2 << " ";

			string res;

			// for each site find the most frequent letter (A/C/G/T)
			for (int i = 0; i < s0.size(); i++)
			{
				map <char, int> mapInf;
				vector <pair<int, char> > inf;

				for (int j = 0; j < h2.size(); j++)
				{
					mapInf[h2[j][i]]++;
				}

				for (auto it : mapInf)
				{
					inf.push_back({ it.second, it.first });
				}

				sort(inf.begin(), inf.end());
				reverse(inf.begin(), inf.end());

				// add the most frequent letter to answer
				res += inf[0].second;
			}

			out << res << "\n";
		}
	}
};



int main()
{
	int m, i;

	in >> m >> S;
	in >> thBeta;
	in >> type;

	for (i = 0; i < m; i++)
	{
		Sol s;

		s.solve(i);
	}


	return 0;
}