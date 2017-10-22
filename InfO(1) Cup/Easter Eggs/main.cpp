#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int query(vector<int> islands);

void dfs(int node, const vector< vector<int> > &edges, vector<int>& order, int father = -1) {
    order.push_back(node);

    for (auto &neighbour : edges[node])
        if (neighbour != father)
            dfs(neighbour, edges, order, node);
}

int findEgg(int N, vector< pair<int, int> > bridges) {
    vector< vector<int> > edges(N);
    for (auto &e : bridges) {
        edges[e.first - 1].push_back(e.second - 1);
        edges[e.second - 1].push_back(e.first - 1);
    }

    vector<int> order;
    dfs(0, edges, order);
    for (auto &x : order)
        ++x;

    int step = 256;
    int answer = 0;
    for (; step; step >>= 1)
        if (answer + step < int(order.size()))
            if (!query(vector<int>(order.begin(), order.begin() + answer + step)))
                answer += step;
    return order[answer];
}

#ifndef EVAL
int X, Q;
vector < int > g[512+10];
 
int main() {
    ifstream fin("input");
    ofstream fout("output");
 
    int n, nr;
    vector < pair < int, int > > v;
    vector < int > Eggs;
 
    fin >> n >> nr; //there are nr Easter Eggs
    for (int i = 1; i <= nr; ++i) {
        int x; fin >> x; //the i-th Easter Eggs
        Eggs.push_back(x);
    }
 
    //the bridges
    for (int i = 1; i < n; ++i) {
        int x, y; fin >> x >> y;
        v.push_back({x, y});
        g[x].push_back(y);
        g[y].push_back(x);
    }
 
    for (vector < int > :: iterator it = Eggs.begin(); it != Eggs.end(); ++it) {
        Q = 0; X = *it;
        fout << findEgg(n, v) << '\n';
        fout << Q << '\n';
        fout << '\n';
    }
}
 
bool used[512+10], is[512+10];
 
void browse(int node) {
    used[node] = 1;
    for (vector < int > :: iterator it = g[node].begin(); it != g[node].end(); ++it) {
        if (used[*it]) continue;
        if (is[*it]) browse(*it);
    }
}
 
bool beautiful(vector < int > v) {
    int n = (int)v.size();
 
    memset(used, 0, sizeof(used));
    memset(is, 0, sizeof(is));
 
    for (int i = 0; i < n; ++i)
        is[v[i]] = 1;
 
    browse(v[0]);
    for (int i = 0; i < n; ++i)
        if (!used[v[i]]) return 0;
    return 1;
}
 
int query(vector < int > v) {
    if (!beautiful(v)) {
        printf("The query-islands are not beautiful");
        exit(0);
    }
 
    bool res = 0; Q++;
    for (vector < int > :: iterator it = v.begin(); it != v.end(); ++it)
        res |= (X == *it);
    return res;
}
#endif
