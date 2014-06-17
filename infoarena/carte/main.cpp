#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    ifstream cin("carte.in");
    ofstream cout("carte.out");

    int T; cin >> T;

    while (T--) {
        string text; cin >> text;
        text = " " + text;
        vector< vector<bool> > can(text.size(), vector<bool>(text.size(), false));

        int N; cin >> N;
        for (int i = 0; i < N; ++i) {
            string S; cin >> S;

            vector<int> prefix(S.size());
            prefix[0] = 0;
            int match = 0;
            for (int j = 1; j < int(S.size()); ++j) {
                while (match && S[match] != S[j])
                    match = prefix[match - 1];

                if (S[match] == S[j])
                    ++match;
                prefix[j] = match;
            }

            match = 0;
            for (int j = 0; j < int(text.size()); ++j) {
                while (match && S[match] != text[j])
                    match = prefix[match - 1];
                if (S[match] == text[j])
                    ++match;
                if (match == int(S.size())) {
                    can[j][match] = true;
                    match = prefix[match - 1];
                }
            }
        }

        vector<int> dp(text.size(), text.size());
        dp[0] = 0;
        for (int i = 1; i < int(text.size()); ++i) {
            dp[i] = 1 + dp[i - 1];
            for (int j = 1; j <= i; ++j)
                if (can[i][j])
                    dp[i] = min(dp[i], dp[i - j]);
        }
        cout << dp.back() << "\n";
    }
}
