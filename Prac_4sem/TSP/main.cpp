#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include "Solver.h"

#define THRESH 1e4
#define MAX_POP 5
#define MAX_CNT 100

using namespace std;

int main() {
    int i, j, n, d;
    double sum=0;
	string s1, s2, filename;
	ifstream f_in;
	cout<<"Insert filename (such as 70_1):"<<endl;
	cin>>filename;

	f_in.open("tsp_" + filename);
	f_in >> s1;
	istringstream os(s1);
	os >> n;

	vector<int> x(n);
	vector<int> y(n);

	for (i=0; i<n; ++i) {
		f_in >> s1 >> s2;
		istringstream os1(s1);
		os1 >> d;
		x[i] = d;

		istringstream os2(s2);
		os2 >> d;
		y[i] = d;
	}
	f_in.close();

	vector<vector<double> > map(n, vector<double>(n, 0));
	for (i=0; i<n; ++i)
		for (j=(i+1); j<n; ++j)
			map[i][j] = map[j][i] = (x[i]-x[j])*(x[i]-x[j]) + (y[i]-y[j])*(y[i]-y[j]);

	Solution S;

	S = solve(map, n);
	for (i=0; i<(n-1); i++) sum += sqrt(map[S.gene[i]][S.gene[i+1]]);

    ofstream f_out("result.txt");
	f_out<<sum<<" "<<1<<"\n";
	for (i=0; i<n; i++) f_out<<S.gene[i]<<" ";
	f_out.close();
	return 0;
}
