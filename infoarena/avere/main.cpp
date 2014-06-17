#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("avere.in");
    ofstream cout("avere.out");

    int S; long long N;

    cin >> S >> N;

    vector< vector<long long> > dp(S + 1, vector<long long>(S + 1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= S; ++i)
        dp[0][i] = 1;
    for (int i = 1; i <= S; ++i)
        for (int j = 1; j <= S; ++j) {
            dp[i][j] = dp[i][j - 1];
            if (j <= i)
                dp[i][j] += dp[i - j][j - 1];
        }

    cout << dp[S][S] << "\n";
    while (S > 0) {
        int i;
        for (i = 1; i <= S; ++i)
            if (dp[S][i] >= N)
                break;
        N -= dp[S][i - 1];
        cout << i << " ";
        S -= i;
    }
}
