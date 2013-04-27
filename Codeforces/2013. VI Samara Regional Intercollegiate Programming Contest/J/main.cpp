#include <iostream>
#include <vector>

using namespace std;

vector< vector<int> > edges;
vector<bool> used;

void dfs(int node) {
    if (used[node])
        return;
    used[node] = true;
    for (auto &son: edges[node])
        dfs(son);
}

int main() {
    int N; cin >> N;

    vector<int> where(N + 1);
    for (int i = 1; i <= N; ++i) {
        int x; cin >> x;
        where[x] = i;
    }

    edges.resize(N + 1);
    int K; cin >> K;
    for (int i = 1; i <= K; ++i) {
        for (int j = 1; j <= N; ++j) {
            int x; cin >> x;
            edges[x].push_back(j);
            edges[j].push_back(x);
        }
    }

    used.resize(N + 1, false);

    int X; cin >> X;
    dfs(where[X]);
    if (used[1])
        cout << "YES\n";
    else
        cout << "NO\n";
}
