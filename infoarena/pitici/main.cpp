#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <tuple>

using namespace std;

void dfs(int node, const vector< vector< pair<int, int> > > &edge, vector<int> &order, vector<bool> &used) {
    if (used[node])
        return;
    used[node] = true;
    for (auto &p : edge[node])
        dfs(p.first, edge, order, used);
    order.push_back(node);
}

int main() {
    ifstream cin("pitici.in");
    ofstream cout("pitici.out");

    int N, M, K; cin >> N >> M >> K;

    vector< vector<pair<int, int> > > edge(N);
    for (int i = 0; i < M; ++i) {
        int x, y, z; cin >> x >> y >> z;
        edge[x - 1].emplace_back(y - 1, z);
    }

    vector<int> order;
    vector<bool> used(N, false);
    dfs(0, edge, order, used);

    vector< vector<int> > dp(N, vector<int>(K, numeric_limits<int>::max() / 2));
    dp[N - 1][0] = 0;
    for (auto &node : order) {
        if (edge[node].empty())
            continue;
        priority_queue< tuple<int, int, int>, vector< tuple<int, int, int> >, greater< tuple<int, int, int> > > heap;

        for (auto &p : edge[node])
            heap.emplace(dp[p.first][0] + p.second, p.first, 0);

        for (int i = 0; i < K; ++i) {
            int distance, to, path;
            tie(distance, to, path) = heap.top();
            dp[node][i] = distance;
            heap.pop();
            heap.emplace(distance - dp[to][path] + dp[to][path + 1], to, path + 1);
        }
    }

    for (int i = 0; i < K; ++i)
        cout << dp[0][i] << " ";
    cout << "\n";
}
