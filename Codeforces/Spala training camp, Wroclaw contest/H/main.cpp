#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    vector<int> X(N), Y(N);
    for (int i = 0; i < N; ++i)
        cin >> X[i] >> Y[i];

    vector< vector<double> > D(N, vector<double>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            D[i][j] = sqrt((X[i] - X[j]) * (X[i] - X[j]) + (Y[i] - Y[j]) * (Y[i] - Y[j]));

    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        D[x][y] = D[y][x] = 0;
    }

    vector<double> dis(N, 1e50);
    vector<int> used(N, 0);
    vector<int> from(N, -1);
    dis[0] = 0;
    for (int i = 0; i < N; ++i) {
        int who = min_element(dis.begin(), dis.end()) - dis.begin();
        used[who] = 1;
        if (from[who] != -1 and dis[who] > 1e-6)
            cout << from[who] + 1 << " " << who + 1 << "\n";
        dis[who] = 1e50;

        for (int j = 0; j < N; ++j)
            if (not used[j])
                if (D[who][j] < dis[j]) {
                    dis[j] = D[who][j];
                    from[j] = who;
                }
    }
}
