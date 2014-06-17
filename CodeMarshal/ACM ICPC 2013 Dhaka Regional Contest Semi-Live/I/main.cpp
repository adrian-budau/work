#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        cout << "Case " << test << ":\n";

        int N, M, source; cin >> N >> M >> source;
        --source;

        vector< vector<pair<int, int>> > edges(N);
        vector< pair<int, int> > edge(M);
        vector<int> costs(M);
        for (int i = 0; i < M; ++i) {
            int x, y; cin >> x >> y;
            --x; --y;
            edges[x].emplace_back(y, i);
            edges[y].emplace_back(x, i);
            cin >> costs[i];

            edge[i] = {x, y};
        }

        sort(costs.begin(), costs.end());

        queue<int> Q;
        Q.push(source);

        vector<bool> used(N, false);
        used[source] = true;

        int K = 0;
        vector<int> answer(M, -1);
        while (!Q.empty()) {
            int now = Q.front();
            Q.pop();

            for (auto &next : edges[now])
                if (not used[next.first]) {
                    answer[next.second] = costs[K++];
                    used[next.first] = true;
                    Q.push(next.first);
                }
        }

        for (int i = 0; i < N; ++i)
            for (auto &next : edges[i])
                if (answer[next.second] == -1)
                    answer[next.second] = costs[K++];

        for (int i = 0; i < M; ++i)
            cout << edge[i].first + 1 << " " << edge[i].second + 1 << " " << answer[i] << "\n";
    }
}
