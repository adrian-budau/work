#include <iostream>
#include <vector>
#include <string>

using namespace std;

int match(string A, string B) {
    vector<int> pi(B.size(), 0);

    int k = 0;
    for (int i = 1; i < int(B.size()); ++i) {
        while (k > 0 and B[k] != B[i])
            k = pi[k - 1];
        if (B[k] == B[i])
            ++k;
        pi[i] = k;
    }

    k = 0;
    int answer = 0;
    for (int i = 0; i < int(A.size()); ++i) {
        while (k > 0 and B[k] != A[i])
            k = pi[k - 1];
        if (B[k] == A[i])
            ++k;
        if (k == int(B.size())) {
            ++answer;
            k = pi[k - 1];
        }
    }

    return answer;
}

int main() {
    int N;
    int test = 0;

    while (cin >> N) {
        ++test;

        string S; cin >> S;

        vector<string> fib(N + 1);
        fib[0] = "0";
        fib[1] = "1";

        vector<long long> dp(N + 1, 0);
        if (S == "0")
            dp[0] = 1;
        if (S == "1")
            dp[1] = 1;
        for (int i = 2; i <= N; ++i) {
            fib[i] = fib[i - 1] + fib[i - 2];
            if (fib[i].size() > 2 * S.size())
                fib[i] = fib[i].substr(0, S.size()) + fib[i].substr(fib[i].size() - S.size());

            dp[i] = dp[i - 1] + dp[i - 2] + match(fib[i - 1].substr(max<int>(fib[i - 1].size() - S.size() + 1, 0)) + fib[i - 2].substr(0, S.size() - 1), S);

            string().swap(fib[i - 2]);
        }

        cout << "Case " << test << ": " << dp[N] << "\n";
    }
}
