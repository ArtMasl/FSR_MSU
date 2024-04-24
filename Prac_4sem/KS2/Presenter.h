#include "Solver.h"
#include<filesystem>
#include<fstream>
namespace  fs = std::filesystem;
using namespace std;

vector<string> filesindir(const fs::path& dir) {
	vector<string> files;
	for (auto& p : fs::recursive_directory_iterator(dir)) {
		if (fs::is_regular_file(p)) {
			files.push_back(p.path().string());
		}
	}
	return files;
}

void fill_result(int& wt, ofstream& out, vector<int>& used_correct_order) {
	out << wt << " " << 0 << '\n';
	for (int i=0; i<sz(used_correct_order); i++)
		out << used_correct_order[i] << " ";
	out << "\n\n";
}
