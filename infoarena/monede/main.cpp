#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

int main() {
    ifstream cin("monede.in");
    ofstream cout("monede.out");

    int N, M; cin >> N >> M;

    int total = 0;
    int open = 0;
    vector< vector<int> > A(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            cin >> A[i][j];
            if (A[i][j]) {
                ++open;
                total += A[i][j];
            }
        }

    int exact = total / open;
    vector< pair<int, int> > need, give;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (A[i][j]) {
                if (A[i][j] < exact)
                    need.emplace_back(i, j);
                if (A[i][j] > exact)
                    give.emplace_back(i, j);
            }

    int source = need.size() + give.size();
    int sink = need.size() + give.size() + 1;
    N = need.size() + give.size() + 2;
    vector< vector<int> > cost(N, vector<int>(N, 0));
    vector< vector<int> > cap(N, vector<int>(N, 0));

    for (int i = 0; i < int(give.size()); ++i) {
        auto p = give[i];
        cap[source][i] = A[p.first][p.second] - exact;
    }

    for (int i = 0; i < int(need.size()); ++i) {
        auto p = need[i];
        cap[i + give.size()][sink] = exact - A[p.first][p.second];
    }

    for (int i = 0; i < int(give.size()); ++i)
        for (int j = 0; j < int(need.size()); ++j) {
            cost[i][give.size() + j] = abs(give[i].first - need[j].first) + abs(give[i].second - need[j].second) + 1;
            cost[give.size() + j][i] = -cost[i][give.size() + j];
            cap[i][give.size() + j] = total;
        }

    int answer = 0;
    vector<int> father(N);

    auto BF = [&]() {
        fill(father.begin(), father.end(), -1);
        father[source] = source;

        vector<int> dist(N);
        dist[source] = 0;

        vector<bool> inQueue(N, false);
        inQueue[source] = true;

        queue<int> Q;
        Q.push(source);
        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            inQueue[node] = false;
            if (node == sink)
                continue;
            for (int next = 0; next < N; ++next)
                if (cap[node][next])
                    if (father[next] == -1 || dist[node] + cost[node][next] < dist[next]) {
                        father[next] = node;
                        dist[next] = dist[node] + cost[node][next];
                        if (!inQueue[next]) {
                            Q.push(next);
                            inQueue[next] = true;
                        }
                    }
        }

        return father[sink] != -1;
    };

    while (BF()) {
        int flow = numeric_limits<int>::max();
        int dist = 0;
        for (int node = sink; node != source; node = father[node]) {
            flow = min(flow, cap[father[node]][node]);
            dist += cost[father[node]][node];
        }
        answer += flow * dist;
        for (int node = sink; node != source; node = father[node]) {
            cap[father[node]][node] -= flow;
            cap[node][father[node]] += flow;
        }
    }

    cout << answer << "\n";
}
