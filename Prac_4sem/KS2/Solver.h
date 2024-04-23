#include <iostream>
#include <vector>
#include <queue>
#include<iterator>
#include<algorithm>

using namespace std;

// struct for item which helps to store weight and value of the item
struct item
{
    int val, wt;
    bool operator==(const item& other) {
        return(other.val == val) && (other.wt == wt);
    }
};
// compares values of the item to sort them later
bool comparison(item i1, item i2) {
    return i1.val  <  i2.val;
}
// sort function, returns a vector because initial vector will be used in the next function
vector<item> sort_vector(vector<item> vec) {
    sort(vec.begin(), vec.end(), comparison);
    return vec;
}
// finds initial order of elements to show which items were used it the process
vector<int> initial_element_order(vector<int>& used, int& n, vector<item>& items, vector<item>& sorted_items) {
    vector<int> used_correct_order(n);//for initial order of elements
    for (int i = 0; i < n; i++) {
        if (used[i] == 1) {
            item elem = sorted_items[i];
            vector<item>::iterator it = find(items.begin(), items.end(), elem);// finds an element in initial vector
            int dist = distance(items.begin(), it);
            used_correct_order[dist] = 1;
        }
    }
    return used_correct_order;
}
// knapsack function takes sorted by values vector of items
// and returns the profit calculated by taken most valued items in knapsack
// until it's full
int knapsack(vector<item>& sorted_items,vector<int> &used,int &n,int &K) {
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
