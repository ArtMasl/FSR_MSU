#include "Presenter.h"

int main() {
    vector<string> files;
    auto cur_p = fs::current_path();
    fs::path dir{ "data" };
    fs::path pathtodata = cur_p / dir;
    files = filesindir(pathtodata);
    for (int file = 0; file < files.size(); file++) {
        ifstream in(files[file]);
        int K, n, val, wt;
        vector<item> items; 
        in >> n >> K;
        for (int i = 0; i < n; i++) {
            in >> val >> wt;
            item a = { val,wt };
            items.push_back(a);
        }
        vector<int> used(n, 0);
        vector<item> sorted_items = sort_vector(items);
        int sum = Solve(sorted_items, used, n, K);
        vector<int> initial_order = initial_element_order(used, n, items, sorted_items);
	files[file].erase(0, 33);
	ofstream out("result_" + files[file] + ".txt");
        fill_result(sum, out, initial_order);
        in.close();
	out.close();
    }
}
