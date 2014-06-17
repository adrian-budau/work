#include <iostream>
#include <fstream>
#include <vector>
#include <array>

using namespace std;

const int kMaxN = 1005;
const int kModulo = 666013;

int main() {
    ifstream cin("frumusete.in");
    ofstream cout("frumusete.out");

    array<array<array<int, 2>, kMaxN>, kMaxN> dp;

    dp[0][0][0] = 1;
    for (int i = 1; i < kMaxN; ++i)
        for (int j = 0; j < i; ++j) {
            dp[i][j][0] = (dp[i - 1][j][0] + dp[i - 1][j][1]) % kModulo;
            dp[i][j][1] = ((j > 0 ? dp[i - 1][j - 1][1] : 0) + dp[i - 1][j][0]) % kModulo;
        }

    int T; cin >> T;

    while (T--) {
        int K; string S;
        cin >> K >> S;

        int N = S.size();
        int answer = 0;
        for (int i = 0; i < N; ++i) {
            if (S[i] == '0')
                continue;
            answer += dp[N - i][K][0];
            answer %= kModulo;
            if (i > 0 and S[i - 1] == S[i])
                --K;
            if (K < 0)
                break;
        }

        if (K == 0)
            answer = (answer + 1) % kModulo;
        cout << answer << "\n";
    }
}

