#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector< vector<int> > get(vector< vector<int> > &dis, int start) {
    vector< vector<int> > dp(1 << dis.size());
    dp[1 << start] = vector<int>(dis.size(), 0x3f3f3f3f);
    dp[1 << start][start] = 0;

    int N = dis.size();
    for (int i = (1 << start) + 1; i < (1 << N); ++i) {
        if (not ((1 << start) & i))
            continue;
        int biti = 0;
        for (int j = 0; j < N; ++j)
            if ((1 << j) & i)
                ++biti;
        if (biti > (N + 1) / 2)
            continue;
        dp[i] = vector<int>(dis.size(), 0x3f3f3f3f);
        for (int j = 0; j < N; ++j) {
            if ((1 << j) & i)
                if (j != start)
                    for (int k = 0; k < N; ++k)
                        if (k != j)
                            if ((1 << k) & i)
                                dp[i][j] = min(dp[i][j], dp[i - (1 << j)][k] + dis[k][j]);
        }
    }

    return dp;
}

int main() {
    int N, M;
    int test = 0;
    while (cin >> N >> M) {
        ++test;
        vector< vector<int> > dis(N, vector<int>(N, 0x3f3f3f3f));
        for (int i = 0; i < M; ++i) {
            int x, y, z; cin >> x >> y >> z;
            dis[x][y] = dis[y][x] = min(dis[x][y], z);
        }

        for (int i = 0; i < N; ++i)
            dis[i][i] = 0;

        for (int k = 0; k < N; ++k)
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);

        vector<vector<int> > dpstart = get(dis, 0);
        vector< vector<int> > dpfinish = get(dis, N - 1);

        int answer = 0x3f3f3f3f;
        for (int i = 0; i < (1 << (N - 2)); ++i) {
            int biti = 0;
            for (int j = 0; j < N; ++j)
                if ((1 << j) & i)
                    ++biti;
            if (biti != (N - 2) / 2)
                continue;

            int stare = i * 2 + 1;
            int aux = 0x3f3f3f3f;
            for (int j = 0; j < N; ++j)
                if ((1 << j) & stare)
                    for (int k = 0; k < N; ++k)
                        if (not ((1 << k) & stare))
                            aux = min(aux, dpstart[stare][j] + dpfinish[(1 << N) - stare - 1][k] + dis[j][k]);
            int aux2 = 0x3f3f3f3f;
            stare = (1 << (N - 2)) - i - 1;
            stare = stare * 2 + 1;
            for (int j = 0; j < N; ++j)
                if ((1 << j) & stare)
                    for (int k = 0; k < N; ++k)
                        if (not ((1 << k) & stare))
                            aux2 = min(aux2, dpstart[stare][j] + dpfinish[(1 << N) - stare - 1][k] + dis[j][k]);
            answer = min(answer, aux + aux2);
        }

        cout << "Case " << test << ": " << answer << "\n";
    }
}

