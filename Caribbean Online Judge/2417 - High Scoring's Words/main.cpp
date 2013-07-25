#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

bool vowel[500];

int main() {
    vowel['a'] = vowel['e'] = vowel['i'] = vowel['o'] = vowel['u'] = true;

    int T; cin >> T;

    while (T--) {
        int C; cin >> C;

        string S;
        for (int i = 0; i < C; ++i) {
            char x; cin >> x;
            S += x;
        }

        sort(S.begin(), S.end());
        reverse(S.begin(), S.end());

        S = " " + S;
        S += ('a' - 1);
        ++C;

        vector< vector< vector<int> > > dp(C + 1, vector< vector<int> >(C, vector<int>(C, -1)));
        vector< vector< vector<int> > > from(C + 1, vector< vector<int> >(C, vector<int>(C, -1)));
        for (int i = 1; i <= C; ++i)
            for (int j = 0; j < C; ++j)
                for (int k = 0; k < C; ++k) {
                    int oldj = j;
                    int oldk = k;
                    if (vowel[int(S[i])])
                        --oldj;
                    else
                        --oldk;
                    if (oldj < 0 || oldk < 0)
                        continue;
                    if (oldj + oldk == 0) {
                        dp[i][j][k] = S[i];
                        from[i][j][k] = 0;
                        continue;
                    }

                    for (int p = 0; p < i; ++p)
                        if (dp[p][oldj][oldk] >= dp[i][j][k]) {
                            dp[i][j][k] = dp[p][oldj][oldk];
                            from[i][j][k] = p;
                        }
                }

        int where = -1, best = -1;
        for (int i = 2; i < C - 1; i += 2)
            if (dp[C][i][i + 1] >= best) {
                best = dp[C][i][i + 1];
                where = i;
            }

        if (best == -1) {
            cout << "-1\n";
            continue;
        }

        string answer;
        int X = C;
        int Y = where;
        int Z = where + 1;
        while (X != 0) {
            answer += S[X];
            int next = from[X][Y][Z];
            if (vowel[int(S[X])])
                --Y;
            else
                --Z;
            X = next;
        }

        answer = answer.substr(1);
        cout << answer << " " << int(answer[answer.size() - 1] - answer[0]) << "\n";
    }
}
