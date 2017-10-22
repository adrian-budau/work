#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int kInfinite = numeric_limits<int>::max() / 2;

int main() {
    int N, K; cin >> N >> K;

    vector< vector<int> > dp(K + 1, vector<int>(N + 1, kInfinite));
    dp[0][1] = 0;

    for (int i = 1; i <= K; ++i) {
        dp[i][1] = 0;
        for (int j = 1; j <= N; ++j)
            for (int k = 1; k < j; ++k)
                dp[i][j] = min(dp[i][j], max(dp[i][j - k], dp[i - 1][k]) + 1);
    }

    int X = 1, Y = N;
    while (X != Y) {
        int N = Y - X + 1;
        int min = kInfinite;
        int where = -1;
        for (int i = 1; i < N; ++i)
            if (max(dp[K][N - i], dp[K - 1][i]) < min) {
                min = max(dp[K][N - i], dp[K - 1][i]);
                where= i;
            }
        cout << "query" << " " << X + where - 1 << "\n";
        cout.flush();
        string answer;
        cin >> answer;
        if (answer == "broke") {
            --K;
            Y = X + where - 1;
        } else {
            X = X + where;
        }
    }
    cout << "answer " << X << "\n";
    cout.flush();
    string S; cin >> S;
}
