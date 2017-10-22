#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);

    int D, N, M; cin >> D >> N >> M;

    vector<int> skip(D + 1, 0);
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        skip[x] = 1;
    }

    vector<vector<int>> leaves(D + 1);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        leaves[y].push_back(x);
    }

    vector<int> dp(D + 1, 2 * M);
    dp[0] = 0;
    for (int i = 1; i <= D; ++i) {
        if (skip[i] == 0)
            dp[i] = dp[i - 1];
        for (auto &start : leaves[i])
            dp[i] = min(dp[i], dp[start - 1] + 1);
    }

    cout << dp.back() << "\n";
}
