#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#define DEBUG
using namespace std;

class Graph {
    int V;
    list<int>* adj_mat;
public:
    Graph(int V) {
        this->V = V;
        adj_mat = new list<int>[V];
    }
    ~Graph() { delete[] adj_mat; }

    void insert_edge(int v_1, int v_2);

    void colouring();
};

void Graph::insert_edge(int v_1, int v_2) {
    adj_mat[v_1].push_back(v_2);
    adj_mat[v_2].push_back(v_1);
}


void Graph::colouring() {
    bool flag;
    int i, j, c, k, colnum=0;
    int* res = new int[V];
    int* var = new int[V];
    bool* available = new bool[V];
    res[0] = 0;

    for (j=1; j<V; j++) res[j] = -1;
    for (c=0; c<V; c++) available[c] = false;

    for (j=1; j<V; j++) {
        list<int>::iterator i;
        for (i = adj_mat[j].begin(); i != adj_mat[j].end(); ++i)
            if (res[*i] != -1)
                available[res[*i]] = true;

        for (c=0; c<V; c++)
            if (!available[c]) break;

        res[j] = c;

        for (i = adj_mat[j].begin(); i != adj_mat[j].end(); ++i)
            if (res[*i] != -1)
                available[res[*i]] = false;
    }

#ifdef DEBUG
    for (j=0; j<V; j++)
        cout<<"Vertex "<<j<<" -- Colour "<<res[j]<<endl;
#endif
    for (i=0; i<V; i++) var[i] = res[0];
    colnum++;

    for (j=1; j<V; j++) {
        flag = false;
        for (k=0; k < V && !flag; k++) {
            if (var[k] == res[j]) flag = true;
        }
        if (!flag) {
            colnum++;
            var[j] = res[j];
        }
    }
    cout<<"The number of Colours: "<<colnum<<endl;
}
