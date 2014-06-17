#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int P, Q, N; cin >> P >> Q >> N;

        vector<int> H(N), G(N);
        for (int i = 0; i < N; ++i)
            cin >> H[i] >> G[i];

        vector<int> K(201, 0), Kprim(201, 0);
        for (int i = 1; i <= 200; ++i)
            if (P >= Q) {
                int laps = i / Q;
                if (i % Q)
                    ++laps;
                K[i] = 0;
                Kprim[i] = laps - 1;
            } else {
                int now = i;
                int many = 0;
                while (now > 0 && (now % Q > P || now % Q == 0)) {
                    now -= P;
                    ++many;
                }

                K[i] = many;
                while (now - Q > 0) {
                    ++Kprim[i];
                    now -= Q;
                }
            }


        vector< vector<int> > dp(N + 1, vector<int>(211 * (N + 5) + 15, numeric_limits<int>::min()));
        auto state = [N](int x) {
            return x + 11 * N + 10;
        };
        dp[0][state(0)] = 0;
        for (int i = 0; i < N; ++i)
            for (int j = -11 * N - 10; j < int(dp[i].size()) - 11 * N - 10; ++j) {
                if (dp[i][state(j)] == numeric_limits<int>::min())
                    continue;
                int k = K[H[i]];
                int kprim = Kprim[H[i]];
                int notake = H[i] / Q + (H[i] % Q > 0);
                // don't take this mob
                dp[i + 1][state(j + notake)] = max(dp[i + 1][state(j + notake)], dp[i][state(j)]);

                // take the mob
                if (k - j > kprim)
                    continue;
                int newstate = state(j - k + kprim - 1);
                if (newstate >= 0 && newstate < int(dp[i].size()))
                    dp[i + 1][newstate] = max(dp[i + 1][newstate], dp[i][state(j)] + G[i]);
            }

        int answer = numeric_limits<int>::min();
        for (int j = -11; state(j) < int(dp[N].size()); ++j)
            answer = max(answer, dp[N][state(j)]);
        cout << "Case #" << test << ": " << answer << "\n";
    }
}
