#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    int N, test = 0;
    while (cin >> N) {
        ++test;

        vector< vector<int> > S(N + 1);
        for (int i = 0; i < N; ++i) {
            int H; cin >> H;
            vector<int> A(H);
            for (int i = 0; i < H; ++i)
                cin >> A[i];
            A.erase(unique(A.begin(), A.end()), A.end());
            S[i] = A;
        }

        vector< vector<int> > are(N + 1, vector<int>(10001, 0));
        vector<int> values;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < int(S[i].size()) - 1; ++j) {
                are[i][S[i][j]] = S[i][j + 1];
                values.push_back(S[i][j]);
            }
            are[i][S[i].back()] = 10001;
            values.push_back(S[i].back());
        }
        values.push_back(0);
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());

        vector< vector<int> > dp(values.size(), vector<int>(N + 1, 0x3f3f3f3f));
        dp[0][N] = 0;
        are[N][0] = 10001;

        for (int i = 0; i < int(values.size()) - 1; ++i)
            for (int j = 0; j <= N; ++j)
                if (dp[i][j] != 0x3f3f3f3f) {
                    int aux = dp[i][j];
                    if (are[j][values[i]] != values[i + 1] and are[j][values[i]] != 10001)
                        aux += 1;

                    int total = 0;
                    int many = 0;
                    for (int k = 0; k <= N; ++k) {
                        if (are[k][values[i + 1]] != 10001 and are[k][values[i + 1]])
                            ++total;
                        if (are[k][values[i + 1]])
                            ++many;
                    }

                    for (int k = 0; k <= N; ++k)
                        if (are[k][values[i + 1]]) {
                            int add = 0;
                            if (k == j and many > 1)
                                ++add;
                            dp[i + 1][k] = min(dp[i + 1][k], aux + total - (are[k][values[i + 1]] != 10001) + add);
                        }
                    continue;
                }
        int aux = 0x3f3f3f3f;
        for (int i = 0; i <= N; ++i)
            aux = min(aux, dp.back()[i]);
        int answer = N + 2 * aux - 1;

        cout << "Case " << test << ": " << answer << "\n";
    }
}

