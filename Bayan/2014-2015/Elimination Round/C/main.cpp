#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <queue>

using namespace std;

static const int dx[4] = {-1, 0, 1, 0};
static const int dy[4] = {0, -1, 0, 1};

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        cout << "Case #" << test  << ":\n";

        int N, M; cin >> N >> M;

        vector< vector<int> > V(N, vector<int>(M));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                cin >> V[i][j];

        vector< vector<int> > edges(N * M);
        vector<int> values(N * M);

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j) {
                for (int k = 0; k < 4; ++k) {
                    int nexti = i + dx[k];
                    int nextj = j + dy[k];
                    if (nexti < 0 || nextj < 0 || nexti >= N || nextj >= M)
                        continue;
                    edges[i * M + j].push_back(nexti * M + nextj);
                }
                values[i * M + j] = V[i][j];
            }

        N *= M;

        vector<int> positions(N);
        for (int i = 0; i < N; ++i)
            positions[i] = i;
        sort(positions.begin(), positions.end(), [&](int x, int y) {
            return values[x] < values[y];
        });

        bool ok = true;
        vector<bool> bad(N, false);
        for (int i = 0; i + 1 < N; ++i) {
            if (i + 1 == N - 1 && values[positions[i + 1]] - values[positions[i]] != 1) {
                ok = false;
                break;
            }

            queue<int> Q;
            Q.push(positions[i]);
            vector<int> distance(N, -1);
            distance[positions[i]] = 0;

            bool found = false;
            while (!Q.empty()) {
                int next = Q.front();
                Q.pop();
                if (next == positions[i + 1]) {
                    if (distance[next] % 2 == (values[positions[i + 1]] - values[positions[i]]) % 2 &&
                        distance[next] <= (values[positions[i + 1]] - values[positions[i]]))
                        found = true;
                    break;
                }

                for (auto &x : edges[next])
                    if (!bad[x] && distance[x] == -1) {
                        distance[x] = distance[next] + 1;
                        Q.push(x);
                    }
            }

            if (!found) {
                ok = false;
                break;
            }
            bad[positions[i]] = true;
        }
        if (ok)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
}
