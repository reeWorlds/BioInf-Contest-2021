#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

ifstream in("test1.txt");
ofstream out("output.txt");

// data for N known humans
// d1, d2 each N binary vectors
// rf2 contains N^2 ternary vectors (0,1,2)
vector <vector <int> > d1, d2, rf2;
// data for M humans (0, 1, 2, -1), -1 indicates unknown
vector <vector <int> > qs;

int n, m;

// sum max value value in each row
int getScore(int arr[3][3])
{
	return max({ arr[0][0], arr[0][1], arr[0][2] }) + max({ arr[1][0], arr[1][1], arr[1][2] })
		+ max({ arr[2][0], arr[2][1], arr[2][2] });
}

// for each row find column with max value
vector <int> buildPredict(int arr[3][3])
{
	vector <int> res(3);

	int mx;

	mx = max({ arr[0][0], arr[0][1], arr[0][2] });
	if (arr[0][0] == mx)
	{
		res[0] = 0;
	}
	else if (arr[0][1] == mx)
	{
		res[0] = 1;
	}
	else
	{
		res[0] = 2;
	}

	mx = max({ arr[1][0], arr[1][1], arr[1][2] });
	if (arr[1][0] == mx)
	{
		res[1] = 0;
	}
	else if (arr[1][1] == mx)
	{
		res[1] = 1;
	}
	else
	{
		res[1] = 2;
	}

	mx = max({ arr[2][0], arr[2][1], arr[2][2] });
	if (arr[2][0] == mx)
	{
		res[2] = 0;
	}
	else if (arr[2][1] == mx)
	{
		res[2] = 1;
	}
	else
	{
		res[2] = 2;
	}

	return res;
}

int main()
{
	in >> n >> m;

	// read input for N humans
	for (int i = 0; i < n; i++)
	{
		vector <int> tDat1; // binary chromosome 1
		vector <int> tDat2; // binary chromosome 1

		string s1, s2;

		in >> s1 >> s2;

		for (int j = 0; j < s1.size(); j++)
		{
			tDat1.push_back(s1[j] - '0');
			tDat2.push_back(s2[j] - '0');
		}

		d1.push_back(tDat1);
		d2.push_back(tDat2);
	}
	// make N^2 children 
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			vector <int> subrf;

			for (int l = 0; l < d1[i].size(); l++)
			{
				subrf.push_back(d1[i][l] + d2[j][l]);
			}

			rf2.push_back(subrf);
		}
	}
	// read input for M humans
	for (int i = 0; i < m; i++)
	{
		vector <int> tDat;
		string s;

		in >> s;

		for (int j = 0; j < s.size(); j++)
		{
			int a = 0;

			if (s[j] == '?')
			{
				a = -1;
			}
			else
			{
				a = s[j] - '0';
			}

			tDat.push_back(a);
		}

		qs.push_back(tDat);
	}

	// find all sites that are known
	vector <int> basePos;
	for (int i = 0; i < qs[0].size(); i++)
	{
		if (qs[0][i] != -1)
		{
			basePos.push_back(i);
		}
	}

	// for each site
	for (int i = 0; i < qs[0].size(); i++)
	{
		// if it is unknown
		if (qs[0][i] == -1)
		{
			// find positions that are close to current site
			// for optimizations only, makes solution worse for smaller thres
			// for example, set thres to 200 t solve test7 in several (~3) minutes with OpenMP
			int thres = 2000000;
			vector <int> myBases;
			for (auto it : basePos)
			{
				if (abs(i - it) < thres)
				{
					myBases.push_back(it);
				}
			}

			// create vector with correlation matrices
			vector <int[3][3]> corr(myBases.size());
			// set all matrices values to 0
			for (auto& it1 : corr)
			{
				for (auto& it2 : it1)
				{
					for (auto& it3 : it2)
					{
						it3 = 0;
					}
				}
			}

			// calculate all matrices
#pragma omp parallel for num_threads(8)
			for (int posI = 0; posI < myBases.size(); posI++)
			{
				int pos = myBases[posI];

				for (int j = 0; j < rf2.size(); j++)
				{
					corr[posI][rf2[j][pos]][rf2[j][i]]++;
				}
			}

			// find best predictor
			int best = 0, bestVal = 0;
			for (int j = 0; j < corr.size(); j++)
			{
				if (getScore(corr[j]) > bestVal)
				{
					bestVal = getScore(corr[j]);
					best = j;
				}
			}

			// build predictor
			vector <int> predict = buildPredict(corr[best]);

			// predict unknown sites
			for (int j = 0; j < qs.size(); j++)
			{
				qs[j][i] = predict[qs[j][myBases[best]]];
			}
		}
	}

	// output result
	for (auto it : qs)
	{
		for (auto itt : it)
		{
			out << itt;
		}
		out << "\n";
	}

	return 0;
}