#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N, M; cin >> N >> M;

    vector<string> S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i];

    vector< vector<int> > dp(N, vector<int>(M, 0));
    dp[0][0] = 1;
    int answer = 1;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (i + j)
                if (S[i][j] == '.') {
                    if (i > 0 && dp[i - 1][j])
                        dp[i][j] = max(dp[i][j], dp[i - 1][j] + 1);
                    if (j > 0 && dp[i][j - 1])
                        dp[i][j] = max(dp[i][j], dp[i][j - 1] + 1);
                    answer = max(answer, dp[i][j]);
                }
    cout << answer << "\n";
}
