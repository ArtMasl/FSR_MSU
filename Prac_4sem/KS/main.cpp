#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#define all(x) (x).begin(), (x).end()

using namespace std;

int main() {
    int i, j, k, p, n, w_max, w_1, sum=0;
    vector<int> v, w, ans_ind;
    vector<vector<int>> V;
	string s1, s2, filename;
	ifstream f_in;
    cout<<"Insert filename (such as 40_0):"<<endl;
	cin>>filename;

    f_in.open("./data/ks_" + filename);
	f_in>>s1>>s2;
	istringstream os1(s1);
	os1>>n;

	istringstream os2(s2);
	os2>>w_max;

    v.resize(n);
	w.resize(n);
	V.resize(n, vector<int>(w_max + 1));

	for (i=0; i<n; i++) {
		f_in>>s1>>s2;
		istringstream os1(s1);
		os1>>p;
		v[i] = p;

		istringstream os2(s2);
		os2>>p;
	    w[i] = p;
	}
	f_in.close();

	for (i=0; i<n; i++) {
		for (j=0; j<=w_max; j++) {
			if (w[i] <= j) {
				if (i > 0)
					V[i][j] = max(V[i-1][j], V[i-1][j-w[i]] + v[i]);
				else
					V[i][j] = v[i];
			}
			else {
				if (i > 0)
					V[i][j] = V[i-1][j];
			}
		}
	}

	k = n-1;
	w_1 = w_max;

	while (V[k][w_1]) {
		if (V[k][w_1] == V[k-1][w_1]) k--;
		else if (V[k][w_1] == (v[k] + V[k-1][w_1 - w[k]])) {
			ans_ind.push_back(k);
			w_1 -= w[k];
			k--;
		}
		else cout<<"Error"<<endl;
	}

	reverse(all(ans_ind));

	for (int i : ans_ind) sum += v[i];

	ofstream f_out("result.txt");
	f_out<<sum<<" "<<0<<"\n";

    k = 0;
    for (i=0; i<n; i++) {
        if (i == ans_ind[k]) {
            f_out<<1<<" ";
            k++;
        }
        else f_out<<0<<" ";
	}
	return 0;
}
