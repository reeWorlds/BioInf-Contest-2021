#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

#define pii pair<int, int>

ifstream in("02.txt");
ofstream out("output.txt");

#define th 25
#define val2 7

class Sol
{
	// genomes without mutation
	vector <string> good;

	// genomes with mutation
	vector <string> bad;

	vector <pii> unOk;

	int n, l;

public:

	void solve(int testNum)
	{
		in >> n >> l;
		// read input genomes
		for (int i = 0; i < n; i++)
		{
			char c;
			string s;

			in >> c >> s;

			if (c == '+')
			{
				bad.push_back(s);
			}
			else
			{
				good.push_back(s);
			}
		}

		// for each genome with mutation
		for (int i = 0; i < bad.size(); i++)
		{
			string sb = bad[i];

			// brutforce each genome without mutation
			for (int j = 0; j < good.size(); j++)
			{
				string sg = good[j];
				int lst = 0;

				// compare good and bad genomes letter by letter
				// find long substrings that are the same for both genomes
				for (int l = 0; l < sg.size(); l++)
				{
					if (sb[l] == sg[l])
					{
						
					}
					else
					{
						// if substring is long enough
						if (l - lst >= th)
						{
							unOk.push_back({ lst, l - 1 });
						}

						lst = l + 1;
					}
				}
				// process last substring
				if (int(sg.size()) - lst >= th)
				{
					unOk.push_back({ lst, int(sg.size()) - 1 });
				}
			}
		}

		// for each site, calculate how many segments cover it
		vector <int> isUnOk(bad[0].size(), 0);
		for (auto it : unOk)
		{
			for (int i = it.first; i <= it.second; i++)
			{
				isUnOk[i]++;
			}
		}

		// find all segments with count less than val2 variable
		vector <pii> anses;
		isUnOk.push_back(1000);

		for (int i = 0; i < isUnOk.size(); i++)
		{
			if (isUnOk[i] < val2)
			{
				int j = i;
				while (isUnOk[j] < val2)
				{
					j++;
				}
				j--;
				anses.push_back({i, j});
				i = j;
			}
		}

		out << "test case = " << testNum << "\n";
		// output those segment and analyze them manualy
		for (auto it : anses)
		{
			out << "{" << it.first << " : " << it.second << "}";
		}

		out << "\n\n";
	}
};



int main()
{
	int n;

	in >> n;

	// go through each testcase
	for (int i = 0; i < n; i++)
	{
		Sol s;

		s.solve(i);
	}

	return 0;
}