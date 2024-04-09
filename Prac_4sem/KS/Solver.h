#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()

using namespace std;

vector<int> Solve(int w_max, vector<int> v, vector<int> w, vector<vector<int>> V) {
    int i, j, k, n, w_1;
    n = sz(v);
    vector<int> ans_ind;
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
	return ans_ind;
}
