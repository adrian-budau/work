#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int kMaxN = 1000;
const int kModulo = 1000000007;

int main() {
    vector< vector<int> > dp(kMaxN + 1, vector<int>(2 * kMaxN + 1, 0));

    dp[0][0] = 1;
    for (int i = 1; i <= kMaxN; ++i)
        for (int j = i; j <= 2 * i; ++j)
            dp[i][j] = (static_cast<int64_t>(dp[i][j - 1]) * (2 * i - j + 1) + dp[i - 1][j - 1] * 2) % kModulo;

    vector< vector<int> > comb(2 * kMaxN + 1, vector<int>(2 * kMaxN + 1, 0));
    comb[0][0] = 1;
    for (int i = 0; i < 2 * kMaxN; ++i)
        for (int j = 0; j <= i; ++j) {
            comb[i + 1][j] = (comb[i + 1][j] + comb[i][j]) % kModulo;
            comb[i + 1][j + 1] = (comb[i + 1][j + 1] + comb[i][j]) % kModulo;
        }

    int N;
    while (cin >> N) {
        if (N == 0)
            break;
        int answer = 0;
        for (int i = 1; i <= N; ++i) {
            int aux = dp[i][2 * i];
            aux = (int64_t(aux) * dp[N - i][2 * (N - i)]) % kModulo;
            aux = (int64_t(aux) * comb[2 * N - 1][2 * i - 1]) % kModulo;
            answer = (answer + aux) % kModulo;
        }
        cout << answer << "\n";
    }
}
