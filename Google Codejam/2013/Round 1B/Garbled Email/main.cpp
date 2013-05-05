#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

map<string, int> M;

vector<pair<string, int> > all(string S) {
    vector<pair<string, int> > answer;
    answer.push_back({S, 0});
    for (int i = 0; i < int(S.size()); ++i) {
        string S2 = S;
        S2[i] = '*';
        answer.push_back({S2, 1});
    }

    for (int i = 0; i < int(S.size()); ++i)
        for (int j = i + 5; j < int(S.size()); ++j) {
            string S2 = S;
            S2[i] = S2[j] = '*';
            answer.push_back({S2, 2});
            for (int k = j + 5; k < int(S.size()); ++k) {
                string S3 = S2;
                S3[k] = '*';
                answer.push_back({S3, 3});
            }
        }

    return answer;
}

void buildDict() {
    ifstream D("dictionary.txt");

    string S;
    int steps = 0;
    while (D >> S) {
        auto l = all(S);
        for (auto &x: l)
            if (M.find(x.first) == M.end())
                M[x.first] = x.second;
            else
                M[x.first] = min(M[x.first], x.second);
        ++steps;
    }
}

int main() {
    buildDict();
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        string S; cin >> S;
        vector< vector<int> > dp(S.size() + 1, vector<int>(7, S.size()));
        dp[0][5] = 0;

        int N = S.size();
        for (int i = 1; i <= N; ++i)
            for (int j = 1; j <= i and j <= 11; ++j) {
                string K = S.substr(i - j, j);
                auto l = all(K);
                for (auto &x: l) {
                    if (M.find(x.first) == M.end())
                        continue;
                    int first = 100, last = -1;
                    for (int p = 0; p < int(x.first.size()); ++p)
                        if (x.first[p] == '*') {
                            first = min(first, p);
                            last = max(last, p);
                        }

                    if (last == -1) {
                        for (int k = 0; k <= 5; ++k)
                            dp[i][min(k + int(x.first.size()), 5)] = min(dp[i][min(k + int(x.first.size()), 5)], dp[i - j][k] + M[x.first]);
                        continue;
                    }

                    last = x.first.size() - last;
                    for (int k = 0; k <= 5; ++k)
                        if (k + first >= 5)
                            dp[i][min(last, 5)] = min(dp[i][min(last, 5)], dp[i - j][k] + M[x.first]);
                }
            }

        int answer = S.size();
        for (int k = 0; k <= 5; ++k)
            answer = min(answer, dp[S.size()][k]);
        cout << "Case #" << test << ": " << answer << "\n";
    }
}
