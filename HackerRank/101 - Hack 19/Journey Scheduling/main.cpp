#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

vector<int> bfs(int node, vector< vector<int> > edges) {
    queue<int> Q;

    vector<int> distance(edges.size(), -1);
    distance[node] = 0;
    Q.push(node);

    while (!Q.empty()) {
        int x = Q.front();
        Q.pop();

        for (auto &y : edges[x])
            if (distance[y] == -1) {
                distance[y] = distance[x] + 1;
                Q.push(y);
            }
    }

    return distance;
}

int main() {
    ios::sync_with_stdio(false);
    int N, M; cin >> N >> M;

    vector< vector<int> > edges(N);
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;

        edges[x - 1].push_back(y - 1);
        edges[y - 1].push_back(x - 1);
    }

    auto first = bfs(0, edges);
    auto second = bfs(max_element(first.begin(), first.end()) - first.begin(), edges);
    auto third = bfs(max_element(second.begin(), second.end()) - second.begin(), edges);

    auto longest = *max_element(second.begin(), second.end());
    while (M--) {
        int V, K; cin >> V >> K;

        cout << 1LL * longest * (K - 1) + max(third[V - 1], second[V - 1]) << "\n";
    }
}
