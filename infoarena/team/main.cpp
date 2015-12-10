#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <cstring>

using namespace std;

static const int kMaxN = 500;
static const int kMaxP = 50;

int dp[kMaxN][kMaxP][kMaxP + 1];
int dist[kMaxN][kMaxN];
int D[kMaxP];

int solve(int node, int from, int to) {
    if (from == to)
        return 0;

    if (dp[node][from][to]  >= 0)
        return dp[node][from][to];

    int answer = numeric_limits<int>::max();
    for (int i = from; i < to; ++i)
        answer = min(answer, solve(D[i], from, i) + solve(D[i], i + 1, to) + dist[D[i]][node]);
    dp[node][from][to] = answer;
    return answer;
}

inline int min(int a, int b) {
    return a < b ? a : b;
}

int main() {
    ifstream cin("team.in");
    ofstream cout("team.out");

    int P, N, M; cin >> P >> N >> M;

    vector< vector< pair<int, int> > > edge(N);
    for (int i = 0; i < M; ++i) {
        int x, y, z; cin >> x >> y >> z;
        --x; --y;
        edge[x].emplace_back(y, z);
        edge[y].emplace_back(x, z);
    }

    for (int i = 0; i < P; ++i) {
        cin >> D[i];
        --D[i];
    }

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            dist[i][j] = numeric_limits<int>::max() / N;

    for (int i = 0; i < N; ++i)
        dist[i][i] = 0;


    for (int i = 0; i < P; ++i) {
        vector<bool> used(N, false);
        priority_queue< pair<int, int> , vector< pair<int, int> >, greater< pair<int, int> > > S;
        S.emplace(0, D[i]);

        while (!S.empty()) {
            int distance, node;
            tie(distance, node) = S.top();
            S.pop();
            if (used[node])
                continue;
            used[node] = true;
            for (auto &e : edge[node])
                if (!used[e.first] && dist[D[i]][e.first] > distance + e.second) {
                    dist[D[i]][e.first] = distance + e.second;
                    S.emplace(dist[D[i]][e.first], e.first);
                }
        }
    }

    memset(dp, -1, sizeof(dp));
    cout << solve(0, 0, P) << "\n";
}

