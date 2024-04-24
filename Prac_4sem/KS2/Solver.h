#include <iostream>
#include <vector>
#include <queue>
#include <iterator>
#include <algorithm>

#define all(x) (x).begin(), (x).end()
#define sz(x) (int)(x).size()


using namespace std;

struct item {
    int val, wt;
    bool operator==(const item& other) {
        return(other.val == val) && (other.wt == wt);
    }
};

bool compare(item A, item B) {
    return A.val  <  B.val;
}

vector<item> sort_vector(vector<item> vec) {
    sort(all(vec), compare);
    return vec;
}

vector<int> initial_element_order(vector<int>& used, int& n, vector<item>& items, vector<item>& sorted_items) {
    vector<int> used_correct_order(n);
    for (int i = 0; i < n; i++) {
        if (used[i] == 1) {
            item elem = sorted_items[i];
            vector<item>::iterator it = find(all(items), elem);// finds an element in initial vector
            int dist = distance(items.begin(), it);
            used_correct_order[dist] = 1;
        }
    }
    return used_correct_order;
}

int Solve(vector<item>& sorted_items,vector<int> &used,int &n,int &K) {
    int cur_weight = 0;
    int cur_profit = 0;
    int iter = 0;
    while (cur_weight <= K && iter<n) {
        cur_weight += sorted_items[iter].wt;
        cur_profit += sorted_items[iter].val;
        iter++;
        used[iter] = true;
    }
    return cur_profit;
}
