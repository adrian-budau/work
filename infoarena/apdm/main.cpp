#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("apdm.in");
    ofstream cout("apdm.out");

    int N, M; cin >> N >> M;

    vector< vector<int> > distance(N, vector<int>(N, N));
    for (int i = 0; i < N; ++i)
        distance[i][i] = 0;
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        distance[x][y] = distance[y][x] = 1;
    }

    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                distance[i][j] = min(distance[i][j], distance[i][k] + distance[k][j]);

    int answer = numeric_limits<int>::max();
    for (int i = 0; i < N; ++i)
        answer = min(answer, 2 * *max_element(distance[i].begin(), distance[i].end()));

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (distance[i][j] == 1) {
                int now = 0;
                for (int k = 0; k < N; ++k)
                    now = max(now, min(distance[i][k], distance[j][k]));
                answer = min(answer, 2 * now + 1);
            }

    cout << answer << "\n";
}
