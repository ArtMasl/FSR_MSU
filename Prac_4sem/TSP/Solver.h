#include <climits>

#define MAX_GEN 1e4
#define MAX_POP 5
#define MAX_CNT 100

using namespace std;

class Solution {
public:
	vector<int> gene;
	double fit;
public:
	Solution() { fit = INT_MAX; }
	friend bool operator<(const Solution& s1, const Solution& s2);
	friend bool operator>(const Solution& s1, const Solution& s2);
};
bool operator<(const Solution& s1, const Solution& s2) {
	return s1.fit < s2.fit;
}
bool operator>(const Solution& s1, const Solution& s2) {
	return s1.fit > s2.fit;
}

bool repeat(vector<int> a, int x) {
    int i;
	for (i=0; i<a.size(); i++) {
		if (a[i] == x) return true;
	}
	return false;
}

vector<int> create_gene(int n) {
    int i, t;
	vector<int> gene;
	gene.resize(n, 0);
	for (i=1; i<n;) {
		t = rand() % (n - 1) + 1;
		if (!repeat(gene, t)) {
			gene[i] = t;
			++i;
		}
	}
	return gene;
}

vector<int> mutate_gene(vector<int> gene, int n, int k) {
	int t, r, r1, c=0;
	while (c < k) {
		r = rand() % (n - 1) + 1;
		r1 = rand() % (n - 1) + 1;
		if (r1 != r) {
			t = gene[r];
			gene[r] = gene[r1];
			gene[r1] = t;
			++c;
		}
	}
	return gene;
}

vector<int> change_ribs(vector<int> gene, int n) {
	int A, B;
	while (true) {
		A = rand() % (n - 2);
		B = rand() % (n - 2);
		if (B < A) swap(A, B);
		if (A + 1 < B) break;
	}

	vector<int> new_gene;
	new_gene = gene;
	new_gene[A + 1] = gene[B];
	int i = 1;
	while (B - i > A) {
		new_gene[A + 1 + i] = gene[B - i];
		++i;
	}
	return new_gene;
}

bool compare(struct Solution s1, struct Solution s2) {
	return s1.fit < s2.fit;
}

double find_fit(vector<int> gene, vector<vector<double>>& map) {
    int i;
	double f = 0;
	for (i=0; i<(gene.size()-1); i++) {
		if (map[gene[i]][gene[i + 1]] == INT_MAX) return INT_MAX;
		f += map[gene[i]][gene[i + 1]];
	}
	return f;
}

Solution solve(vector<vector<double>>& map, int n) {
	int i, c=0, gen=1;

	vector<Solution> pop;
	Solution t;
    t.gene = create_gene(n);
    t.fit = find_fit(t.gene, map);
    pop.push_back(t);

	while (gen <= MAX_GEN) {
		if (gen % 1000 == 0) sort(pop.begin(), pop.end(), compare);
		while (pop.size() >= MAX_POP) {
			pop.erase(pop.begin() + (pop.size()/2), pop.end());
		}
		for (i=0; i<pop.size(); i++) {
			Solution p1 = pop[i];
            c = 0;
			while (true) {
				vector<int> new_g;
				new_g = change_ribs(p1.gene, n);
				Solution new_gene;
				new_gene.gene = new_g;
				new_gene.fit = find_fit(new_gene.gene, map);

				if (new_gene.fit <= pop[i].fit) {
					pop[i] = new_gene;
					break;
				}
				else if (c > MAX_CNT) {
					t.gene = mutate_gene(new_gene.gene, n, rand() % 100 + 5);
					t.fit = find_fit(t.gene, map);
					pop.push_back(t);
					break;
				}
				++c;
			}
		}
        gen++;
	}
	sort(pop.begin(), pop.end(), compare);
	return pop[0];
}
