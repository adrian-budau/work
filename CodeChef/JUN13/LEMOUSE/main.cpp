#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    int T; cin >> T;

    while (T--) {
        int N, M; cin >> N >> M;

        vector< vector<int> > A(N + 2, vector<int>(M + 2, 0));
        for (int i = 1; i <= N; ++i) {
            string S; cin >> S;
            for (int j = 0; j < M; ++j)
                if (S[j] == '1')
                    A[i][j + 1] = 1;
        }

        vector< vector< vector<int> > > dp(N + 2, vector< vector<int> >(M + 2, vector<int>(2, N * M)));
        dp[0][1][0] = A[1][1];
        for (int i = 1; i <= N; ++i)
            for (int j = 1; j <= M; ++j) {
                dp[i][j][0] = min(dp[i][j - 1][0] + A[i - 1][j] + A[i][j + 1] + A[i + 1][j], dp[i][j - 1][1] + A[i + 1][j] + A[i][j + 1]);
                dp[i][j][1] = min(dp[i - 1][j][0] + A[i + 1][j] + A[i][j + 1], dp[i - 1][j][1] + A[i + 1][j] + A[i][j + 1] + A[i][j - 1]);
            }

        cout << min(dp[N][M][0], dp[N][M][1]) << "\n";
    }
}
