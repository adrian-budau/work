#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>

using namespace std;

int abs(int x) {
    if (x < 0)
        return -x;
    return x;
}

int main() {
    int N, Z, W; cin >> N >> Z >> W;
    vector<int> A(N + 2);
    A[0] = W;
    A[1] = Z;
    for (int i = 2; i < N + 2; ++i)
        cin >> A[i];

    vector< vector<int> > dp[2];
    dp[0] = vector< vector<int> >(N + 2, vector<int>(N + 2, 0));
    dp[1] = vector< vector<int> >(N + 2, vector<int>(N + 2, numeric_limits<int>::max()));

    for (int i = 0; i < N + 1; ++i)
        dp[0][N + 1][i] = dp[1][N + 1][i] = abs(A[i] - A[N + 1]);

    for (int i = N; i >= 0; --i)
        for (int j = i - 1; j >= 0; --j) {
            dp[0][i][j] = max(dp[0][i + 1][j], dp[1][i + 1][i]);
            dp[1][i][j] = min(dp[1][i + 1][j], dp[0][i + 1][i]);
        }

    cout << dp[0][2][0] << "\n";
}
