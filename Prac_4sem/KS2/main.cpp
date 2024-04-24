#include "Presenter.h"

int main() {
    vector<string> files;
    auto cur_p = fs::current_path();
    fs::path dir{ "data" };
    fs::path pathtodata = cur_p / dir;
    files = filesindir(pathtodata);
    ofstream out("result.txt");
    for (int file = 0; file < files.size(); file++) {
        ifstream in(files[file]);
        //ifstream in;
        //in.open(files[file]);
        //ofstream out("result_" + files[file] + ".txt");
        //ofstream out;
        //out.open("result_" + files[file] + ".txt");
        int K, n, val, wt;
        //creation of given list of items
        vector<item> items; 
        in >> n >> K;
        for (int i = 0; i < n; i++) {
            in >> val >> wt;
            item a = { val,wt };
            items.push_back(a);
        }
        vector<int> used(n, 0);
        vector<item> sorted_items = sort_vector(items);
        int total_profit = knapsack(sorted_items, used, n, K);
        cout << total_profit << endl;
        /*vector<int> initial_order = initial_element_order(used, n, items, sorted_items);// uncommenting will reveal the used items for filling knapsack
        show1d_matrix<int>(initial_order);*/
        fill_result(total_profit, out, files[file], used);
        in.close();
    }
    out.close();
}
