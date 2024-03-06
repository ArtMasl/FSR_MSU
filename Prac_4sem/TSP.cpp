// TSP, genetic algorithm or ant optimization
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "tspSolver.h"


#define POP_SIZE 1
#define THRESH 10000
#define MAX_POP 5
#define MAX_CNT 100

using namespace std;

int main() {
	string s1, s2;
	ifstream File;

	string name;
	cout << "Enter file number:\n(for example: 1000_1)\n";
	cin >> name;
	if (name == "1000_1") cout << "wait long please\n";



	File.open("./data/tsp_" + name);
	int n;
	File >> s1;
	istringstream os(s1);
	os >> n;



	vector<int> x(n);
	vector<int> y(n);

	int d;
	for (int i = 0; i < n; ++i) {
		File >> s1 >> s2;
		istringstream os1(s1);
		os1 >> d;
		x[i] = d;

		istringstream os2(s2);
		os2 >> d;
		y[i] = d;
	}
	File.close();

	//Distances between nodes
	vector<vector<double> > map(n, vector<double>(n, 0)); 
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			map[i][j] = map[j][i] = pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2);//SQUARED distances for better accuracy
		}
	}


	individual A;

	A = solve(map, n);
	double sum = 0;
	for (int i = 0; i < n - 1; i++) {
		double temp;
		temp = map[A.gene[i]][A.gene[i + 1]];
		sum += sqrt(temp);
	}


	cout << "Summary path:" << sum << endl;
	return 0;
}
