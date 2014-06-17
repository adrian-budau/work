#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
class Graph {
  public:
    Graph(const int &nodes = 0) {
        edges.resize(nodes);
        father.resize(nodes, 0);
        longest.resize(nodes, 1);
        critical.resize(nodes, 0);
    }
 
    void addEdge(const int &x, const int &y) {
        edges[x].push_back(y);
        edges[y].push_back(x);
    }
 
    int best() {
        build(0);
        return find(0);
    }
 
  private:
    void build(const int &);
    int find(const int &);
 
    vector<int> father;
    vector<int> longest;
    vector<int> critical;
    vector< vector<int> > edges;
};
 
class functor {
  public:
    functor(const vector<int> &_buffer): buffer(_buffer) {}
     
    bool operator()(const int &x, const int &y) const {
        return buffer[x] < buffer[y];
    }
  private:
    const vector<int> &buffer;
};
 
void Graph::build(const int &node) {
    for (vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it)
        if (father[node] != *it) {
            father[*it] = node;
            build(*it);
        }
 
    sort(edges[node].begin(), edges[node].end(), functor(longest));
     
    for (vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it)
        if (father[node] != *it) {
            if (longest[*it] >= longest[node])
                ++longest[node];
            else
                critical[node] = longest[node];
        }
}
 
int Graph::find(const int &node) {
    int answer = longest[node];
 
    for (vector<int>::iterator it  = edges[node].begin(); it != edges[node].end(); ++it)
        if (father[node] != *it) {
            int aux = longest[node];
            if (longest[*it] > critical[node])
                --longest[node];
 
            if (longest[node] > critical[*it])
                ++longest[*it];
             
            answer = max(answer, find(*it));
            longest[node] = aux;
        }
 
    return answer;
}
 
int main() {
    ifstream cin("trib.in");
    ofstream cout("trib.out");
 
    int N; cin >> N;
     
    Graph G(N);
 
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        G.addEdge(x, y);
    }
 
    cout << G.best() << "\n";
}
