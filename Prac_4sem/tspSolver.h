#define POP_SIZE 1
#define THRESH 10000
#define MAX_POP 5
#define MAX_CNT 100
//#define DEBUG
using namespace std;

class individual {
public:
	vector<int> gene;
	double fitness;
public:
	individual() {
		fitness = INT_MAX;
	}
	void print() {
#ifdef DEBUG
		cout << "GENE: [ ";
#endif
		for (int i : gene)
			cout << i << " ";
#ifdef DEBUG
		cout << "]\n";
		cout << "FITNESS: " << fitness << endl;
#endif
	}
	friend bool operator<(const individual& c1, const individual& c2);
	friend bool operator>(const individual& c1, const individual& c2);
};
bool operator<(const individual& c1, const individual& c2) {
	return c1.fitness < c2.fitness;
}
bool operator>(const individual& c1, const individual& c2) {
	return c1.fitness > c2.fitness;
}

bool repeat(vector<int> a, int x) {
	for (int i = 0; i < a.size(); i++) {
		if (a[i] == x)
			return true;
	}
	return false;
}

vector<int> generate_gene(int n) {
	vector<int> gene;
	gene.resize(n, 0);
	for (int i = 1; i < n;) {
		int temp = rand() % (n - 1) + 1;
		if (!repeat(gene, temp)) {
			gene[i] = temp;
			++i;
		}
	}
	return gene;
}

vector<int> mutate_gene(vector<int> gene, int n, int k) {
	int cnt = 0;
	while (cnt < k) {
		int r = rand() % (n - 1) + 1;
		int r1 = rand() % (n - 1) + 1;
		if (r1 != r) {
			int temp = gene[r];
			gene[r] = gene[r1];
			gene[r1] = temp;
			++cnt;
		}
	}
	return gene;
}

vector<int> change_ribs(vector<int> gene, int n) {
	int A, B;
	while (true) {
		A = rand() % (n - 2);
		B = rand() % (n - 2);
		if (B < A)
			swap(A, B);
		if (A + 1 < B)
			break;
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

bool lessthan(struct individual t1, struct individual t2) {
	return t1.fitness < t2.fitness;
}

double find_fitness(vector<int> gene, vector<vector<double> >& map)
{
	double f = 0;
	for (int i = 0; i < gene.size() - 1; i++) {
		if (map[gene[i]][gene[i + 1]] == INT_MAX)
			return INT_MAX;
		f += map[gene[i]][gene[i + 1]];
	}
	return f;
}

individual solve(vector<vector<double> >& map, int n) {
	int gen = 1;

	vector<individual> pop;
	individual temp;

#ifdef DEBUG
		cout << "Generating gene number: " << i << "/10\n";
#endif
		temp.gene = generate_gene(n);
		temp.fitness = find_fitness(temp.gene, map);
		pop.push_back(temp);

#ifdef DEBUG
	cout << "Created initial population\n";
#endif

	while (gen <= THRESH) {
		if (gen % 1000 == 0)
#ifdef DEBUG
			cout << "Current gen: " << gen << endl;
#endif
		sort(pop.begin(), pop.end(), lessthan);
		while (pop.size() >= MAX_POP) {
			pop.erase(pop.begin() + (pop.size() / 2), pop.end());
		}
		if (gen % 1000 == 0) {
#ifdef DEBUG
			cout << "Pop size: " << pop.size() << endl;
			cout << "Best fitness: " << pop[0].fitness << endl;
#endif
		}

		for (int i = 0; i < pop.size(); i++) {
			individual p1 = pop[i];

			int cnt = 0;
			while (true) {

				vector<int> new_g;
				new_g = change_ribs(p1.gene, n);
				individual new_gene;
				new_gene.gene = new_g;
				new_gene.fitness = find_fitness(new_gene.gene, map);

				if (new_gene.fitness <= pop[i].fitness) {
					pop[i] = new_gene;
					break;
				}
				else if (cnt > MAX_CNT) {
					temp.gene = mutate_gene(new_gene.gene, n, rand() % 100 + 5);
					temp.fitness = find_fitness(temp.gene, map);
					pop.push_back(temp);
					break;
				}
				++cnt;
			}
		}

		gen++;
	}

	sort(pop.begin(), pop.end(), lessthan);

	return pop[0];

}

