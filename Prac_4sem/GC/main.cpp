#include <iostream>
#include <fstream>
#include <sstream>
#include "Solver.h"

using namespace std;

int main() {
    int i, e1, e2, V, E;
    string s1, s2, filename;
    ifstream f_in;

    cout<<"Insert filename (such as 40_0):"<<endl;
    cin>>filename;

    f_in.open("./data/gc_" + filename);
    f_in>>s1>>s2;
    istringstream os1(s1);
    os1>>V;

    istringstream os2(s2);
    os2>>E;

    Graph G(V);

    for (i=0; i<E; ++i) {
        f_in>>s1>>s2;
        istringstream os3(s1);
        os3>>e1;
        istringstream os4(s2);
        os4>>e2;
        G.insert_edge(e1, e2);
    }

    f_in.close();

    G.colouring();

    return 0;
}
