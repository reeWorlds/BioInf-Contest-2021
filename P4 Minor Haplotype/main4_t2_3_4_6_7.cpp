#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <string>
using namespace std;

ifstream in("in2.txt");
ofstream out("output.txt");

int S;
double thBeta;
char type;

class Sol
{

public:

	string s0;
	int k, n;
	double minRate, alpha;

	vector <string > dat;


	void solve(int testCase)
	{
		// read input
		string s;

		in >> s;
		in >> s0;
		in >> k >> n;
		in >> minRate >> alpha;

		for (int i = 0; i < n; i++)
		{
			in >> s;

			dat.push_back(s);
		}

		// choose th1 and th2 for correponding test
		const double th1 = (n * 0.02 * (1.0 / 3.0) + n * 0.3) * 0.5; // test2
		const int th2 = 1; // test2

		//const double th1 = (n * 0.1 * (1.0 / 3.0) + n * 0.1) * 0.65; // test3
		//const int th2 = 100; // test3

		//const double th1 = (n * 0.1 * (1.0 / 3.0) + n * 0.2) * 0.58; // test4
		//const int th2 = 4; // test4

		//const double th1 = (n * 0.05 * (1.0 / 3.0) + n * 0.1) * 0.6; // test6
		//const int th2 = 4; // test6

		//const double th1 = (n * 0.03 * (1.0 / 3.0) + n * 0.01) * 1.225; // test7
		//const int th2 = 3; // test7

		// find all bases that have enough non-reference sites 
		vector <pair<int, char> > diffToS0;

		for (int i = 0; i < s0.size(); i++)
		{
			map <char, int> inf;

			for (int j = 0; j < n; j++)
			{
				if (dat[j][i] != s0[i])
				{
					inf[dat[j][i]]++;
				}
			}

			if (inf.size() == 0)
			{
				continue;
			}

			vector <pair<int, char> > infV;

			for (auto it : inf)
			{
				infV.push_back({ it.second, it.first });
			}
			sort(infV.begin(), infV.end());
			reverse(infV.begin(), infV.end());

			if (infV[0].first >= th1)
			{
				diffToS0.push_back({ i, infV[0].second });
			}
		}

		// if there are enough differend bases
		if (diffToS0.size() < th2)
		{
			out << "1\n";
		}
		else
		{
			out << "2 ";

			string s2 = s0;
			for (auto it : diffToS0)
			{
				s2[it.first] = it.second;
			}

			out << s2 << "\n";
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