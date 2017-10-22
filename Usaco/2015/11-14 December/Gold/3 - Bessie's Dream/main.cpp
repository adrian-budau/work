#include <cstdio>
#include <vector>
#include <array>
#include <queue>
#include <limits>
#include <algorithm>
#include <tuple>
#include <cstring>
#include <iostream>
#include <set>
#include <cassert>

using namespace std;

static const int kInfinite = 0x3f3f3f3f;
static const int kMaxN = 1005;

int color[kMaxN][kMaxN];
vector< pair<int, int> > edges[kMaxN * kMaxN];
int dist[2][kMaxN * kMaxN];

int main() {
    freopen("dream.in", "r", stdin);
    freopen("dream.out", "w", stdout);

    int N, M; scanf("%d%d", &N, &M);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            scanf("%d", &color[i][j]);

    assert(color[0][0] == 1 && color[N - 1][M - 1] == 1);

    // left
    for (int i = 0; i < N; ++i)
        for (int j = 0, last = -1; j < M; ++j) {
            if (color[i][j] == 0) {
                last = -1;
                continue;
            }
            if (last != -1)
                edges[i * M + j].emplace_back(i * M + last, j - last);
            if (last == -1 || color[i][j] != 4)
                last = j;
        }
    // up
    for (int j = 0; j < M; ++j)
        for (int i = 0, last = -1; i < N; ++i) {
            if (color[i][j] == 0) {
                last = -1;
                continue;
            }
            if (last != -1)
                edges[i * M + j].emplace_back(last * M + j, i - last);
            if (last == -1 || color[i][j] != 4)
                last = i;
        }
    // right
    for (int i = 0; i < N; ++i)
        for (int j = M - 1, last = -1; j >= 0; --j) {
            if (color[i][j] == 0) {
                last = -1;
                continue;
            }
            if (last != -1)
                edges[i * M + j].emplace_back(i * M + last, last - j);
            if (last == -1 || color[i][j] != 4)
                last = j;
        }
    // down
    for (int j = 0; j < M; ++j)
        for (int i = N - 1, last = -1; i >= 0; --i) {
            if (color[i][j] == 0) {
                last = -1;
                continue;
            }
            if (last != -1)
                edges[i * M + j].emplace_back(last * M + j, last - i);
            if (last == -1 || color[i][j] != 4)
                last = i;
        }

    memset(dist, 0x3f3f3f3f, sizeof(dist));
    dist[0][0] = 0;
    set< pair<int, pair<int, int> > > S;
    S.emplace(0, make_pair(0, 0));

    while (!S.empty()) {
        int distance, oranges, node;
        distance = S.begin()->first;
        tie(oranges, node) = S.begin()->second;
        S.erase(S.begin());
        for (auto &e : edges[node]) {
            int new_oranges = oranges;
            if (color[e.first / M][e.first % M] == 4 || e.second > 1)
                new_oranges = 0;
            if (color[e.first / M][e.first % M] == 2)
                new_oranges = 1;
            if (color[e.first / M][e.first % M] == 3 && !new_oranges)
                continue;
            if (dist[new_oranges][e.first] > distance + e.second) {
                S.erase(make_pair(dist[new_oranges][e.first], make_pair(new_oranges, e.first)));
                dist[new_oranges][e.first] = distance + e.second;
                S.emplace(distance + e.second, make_pair(new_oranges, e.first));
            }
        }
    }

    int answer = min(dist[0][N * M - 1], dist[1][N * M - 1]);
    if (answer == kInfinite)
        answer = -1;
    cout << answer << "\n";
}
