#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int T; cin >> T;

    for (int i = 1; i <= T; ++i) {
        int K, N; cin >> K >> N;

        vector<int> A(200, 0);
        for (int i = 0; i < K; ++i) {
            int x; cin >> x;
            A[x - 1] ++;
        }

        vector<int> type(N);
        vector< vector<int> > keys(N);
        for (int i = 0; i < N; ++i) {
            cin >> type[i]; --type[i];
            int many; cin >> many;
            keys[i].reserve(many);
            for (int j = 0; j < many; ++j) {
                int x; cin >> x; --x;
                keys[i].push_back(x);
            }
        }

        vector<bool> dp(1 << N, false);
        dp[0] = true;
        for (int i = 1; i < (1 << N); ++i) {
            vector<int> B = A;
            for (int j = 0; j < N; ++j)
                if (((1 << j) & i) == 0) {
                    --B[type[j]];
                    for (auto &key: keys[j])
                        ++B[key];
                }

            if (*min_element(B.begin(), B.end()) < 0)
                continue;

            for (int j = 0; j < N; ++j)
                if ((1 << j) & i)
                    if (B[type[j]] > 0 && dp[i - (1 << j)]) {
                        dp[i] = true;
                        break;
                    }

            //cerr << i << " -> " << boolalpha << dp[i] << "\n";
        }

        cout << "Case #" << i << ": ";
        if (dp[(1 << N) - 1] == false) {
            cout << "IMPOSSIBLE\n";
            continue;
        }

        vector<int> answer;
        int state = (1 << N) - 1;
        for (int i = 0; i < N; ++i) {
            vector<int> B = A;
            for (int j = 0; j < N; ++j)
                if (((1 << j) & state) == 0) {
                    --B[type[j]];
                    for (auto &key: keys[j])
                        ++B[key];
                }

            int next = -1;
            for (int j = 0; j < N; ++j)
                if ((1 << j) & state)
                    if (B[type[j]] > 0 && dp[state - (1 << j)]) {
                        next = j;
                        break;
                    }
            state -= (1 << next);
            answer.push_back(next + 1);
        }

        for (auto &x : answer)
            cout << x << " ";
        cout << "\n";
    }
}
