#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;


int main() {
    ifstream cin("expresii2.in");
    ofstream cout("expresii2.out");

    int N, K; int64_t P;
    cin >> N >> K >> P;

    vector< vector< vector<int64_t> > > dp(N + 1, vector< vector<int64_t> >(N + 1, vector<int64_t>(3, 0)));
    dp[0][1][0] = 1;
    for (int i = 1; i <= N; ++i)
        for (int j = 0; j <= N; ++j) {
            if (j < N)
                dp[i][j][0] += dp[i - 1][j + 1][0] + dp[i - 1][j + 1][1] + dp[i - 1][j + 1][2];
            if (j > 1)
                dp[i][j][1] += dp[i - 1][j - 1][0] + dp[i - 1][j - 1][1] + dp[i - 1][j - 1][2];

            if (j > 0)
                dp[i][j][2] = dp[i - 1][j][0] + dp[i - 1][j][1] + dp[i - 1][j][2];

            dp[i][j][0] *= K;
            dp[i][j][1] *= 2;
        }

    cout << dp[N][0][0] + dp[N][0][1] + dp[N][0][2] << "\n";

    int variables = 0;
    for (int i = 0; i < N; ++i) {
        int now = 0;
        while (now < K and dp[N - i][variables][0] / K < P) {
            P -= dp[N - i][variables][0] / K;
            ++now;
        }

        if (now < K) {
            cout << char('A' + now);
            ++variables;
            continue;
        }

        while (now < K + 2 and dp[N - i][variables][1] / 2 < P) {
            P -= dp[N - i][variables][1] / 2;
            ++now;
        }

        if (now < K + 2) {
            if (now == K)
                cout << "+";
            else
                cout << "*";
            --variables;
            continue;
        }

        cout << "!";
    }
}
