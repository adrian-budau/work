#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;
static const int kInv2 = (kModulo + 1) / 2;

int main() {
    ios::sync_with_stdio(false);

    int N; cin >> N;
    vector< vector<int> > dp(N, vector<int>(4, 0));
    dp[0][0] = 1;
    for (int i = 1; i < N; ++i)
        for (int j = 1; j <= 3; ++j) {
            dp[i][j] = (dp[i - 1][j - 1] + 1LL * (j + i - 1) * dp[i - 1][j]) % kModulo;
        }

    int answer = 1LL * N * (N - 1) / 2 * (1LL * dp[N - 2][2] + 1LL * dp[N - 2][3]) % kModulo;

    int with_1 = 1LL * (N - 1) * dp[N - 2][1] % kModulo;
    int with_2 = 1LL * N * (1LL * dp[N - 1][2] - with_1) % kModulo;
    with_2 = 1LL * with_2 * kInv2 % kModulo;
    with_2 = 1LL * with_2 * kInv2 % kModulo;

    if (with_2 < 0)
        with_2 += kModulo;

    answer = (1LL * answer + 1LL * with_2) % kModulo;

    cout << answer << "\n";

}
