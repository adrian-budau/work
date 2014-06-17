#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

const int kMaxN = 21;

int dp[kMaxN][kMaxN][kMaxN][kMaxN];

int main() {
    int T; cin >> T;

    while (T--) {
        int N, M; cin >> N >> M;

        vector<string> S(N);
        for (int i = 0; i < N; ++i)
            cin >> S[i];

        vector< vector<int> > U(N, vector<int>(M, 0)), D = U, L = U, R = U;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j) {
                if (S[i][j] == '^')
                    U[i][j] = L[i][j] = 1;
                else
                    continue;
                if (i > 0)
                    U[i][j] += U[i - 1][j];
                if (j > 0)
                    L[i][j] += L[i][j - 1];
            }

        for (int i = N - 1; i >= 0; --i)
            for (int j = M - 1; j >= 0; --j) {
                if (S[i][j] == '^')
                    D[i][j] = R[i][j] = 1;
                else
                    continue;
                if (i < N - 1)
                    D[i][j] += D[i + 1][j];
                if (j < M - 1)
                    R[i][j] += R[i][j + 1];
            }

        vector< vector<int> > V(N, vector<int>(M));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if (min({U[i][j], L[i][j], D[i][j], R[i][j]}) >= 3)
                    V[i][j] = 1;
                else
                    V[i][j] = 0;

        memset(dp, 0, sizeof(dp));
        auto get = [&](int x1, int y1, int x2, int y2) {
            if (x1 > x2 || y1 > y2)
                return 0;
            return dp[x1][y1][x2][y2];
        };

        for (int o = 0; o < N; ++o)
            for (int v = 0; v < M; ++v)
                for (int i = 0; i + o < N; ++i)
                    for (int j = 0; j + v < M; ++j) {
                        vector<int> mex(4 * (o + 1) * (v + 1) + 1, 0);
                        for (int k = i; k <= i + o; ++k)
                            for (int l = j; l <= j + v; ++l)
                                if (V[k][l] == 1)
                                    mex[get(i, j, k - 1, l - 1) ^ get(i, l + 1, k - 1, j + v)
                                      ^ get(k + 1, j, i + o, l - 1) ^ get(k + 1, l + 1, i + o, j + v)] = 1;
                        for (int k = 0; k <= 4 * (o + 1) * (v + 1); ++k)
                            if (mex[k] == 0) {
                                dp[i][j][i + o][j + v] = k;
                                break;
                            }
                    }

        if (dp[0][0][N - 1][M - 1] == 0)
            cout << "Kirito\n";
        else
            cout << "Asuna\n";        
    }
}
