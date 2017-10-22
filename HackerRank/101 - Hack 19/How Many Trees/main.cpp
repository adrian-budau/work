#include <iostream>
#include <vector>

using namespace std;

const int kModulo = 1000 * 1000 * 1000 + 7;

int pow(int a, int b) {
    if (b == 0)
        return 1;
    if (b % 2)
        return 1LL * pow(a, b - 1) * a % kModulo;

    int v = pow(a, b / 2);
    return 1LL * v * v % kModulo;
}

int main() {
    int N, L; cin >> N >> L;

    if (N == 2) {
        cout << 0 << "\n";
        return 0;
    }

    vector<int> fact(N + 1, 1);
    for (int i = 1; i <= N; ++i)
        fact[i] = 1LL * fact[i - 1] * i % kModulo;

    vector<int> ifact(N + 1, 1);
    for (int i = 0; i <= N; ++i)
        ifact[i] = pow(fact[i], kModulo - 2);

    auto comb = [&](int N, int K) {
        int aux = 1LL * fact[N] * ifact[K] % kModulo;
        return 1LL * aux * ifact[N - K] % kModulo;
    };

    vector<int> dp(N - L + 1);
    dp[1] = 1;
    for (int i = 2; i <= N - L; ++i) {
        dp[i] = pow(i, N - 2);
        for (int j = 1; j < i; ++j)
            dp[i] = (dp[i] - 1LL * dp[j] * comb(i, j)) % kModulo;
    }

    int answer = 1LL * dp[N - L] * comb(N, L) % kModulo;
    if (answer < 0)
        answer += kModulo;
    cout << answer << "\n";
}
