#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

static const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, -1, 0, 1};

int main() {
    ifstream cin("lightson.in");
    ofstream cout("lightson.out");

    int N, M; cin >> N >> M;

    vector< vector< vector< pair<int, int> > > > switches(N, vector< vector< pair<int, int> > >(N));
    for (int i = 0; i < M; ++i) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        switches[x1 - 1][y1 - 1].emplace_back(x2 - 1, y2 - 1);
    }

    queue< pair<int, int> > Q;
    vector< vector<bool> > good(N, vector<bool>(N, false)), visited = good;
    good[0][0] = true;
    visited[0][0] = true;
    Q.emplace(0, 0);

    int answer = 1;
    while (!Q.empty()) {
        int x, y; tie(x, y) = Q.front(); Q.pop();

        for (auto &room : switches[x][y]) {
            if (good[room.first][room.second])
                continue;
            good[room.first][room.second] = true;
            ++answer;
            if (visited[room.first][room.second])
                Q.emplace(room);
        }

        for (int k = 0; k < 4; ++k)
            if (x + dx[k] >= 0 && x + dx[k] < N && y + dy[k] >= 0 && y + dy[k] < N)
                if (!visited[x + dx[k]][y + dy[k]]) {
                    visited[x + dx[k]][y + dy[k]] = true;
                    if (good[x + dx[k]][y + dy[k]])
                        Q.emplace(x + dx[k], y + dy[k]);
                }
    }

    cout << answer << "\n";
}
