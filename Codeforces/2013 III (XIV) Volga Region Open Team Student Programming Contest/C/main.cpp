#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int N, M; cin >> N >> M;

    vector< vector<int> > A(N + 1, vector<int>(M + 1, 0));

    vector<string> S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i];

    int step;
    for (step = 1; step <= N + M; step <<= 1);

    int answer;
    auto getDP = [&](vector< vector<int> > &dp, vector< vector<int> > &from) -> int {
        dp[0][0] = int(S[0][0] != '.');
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if (dp[i][j] <= answer) {
                    if (i + 1 < N) {
                        int tmp = int(S[i + 1][j] != '.');
                        if (S[i][j] == S[i + 1][j])
                            tmp += dp[i][j];
                        if (tmp < dp[i + 1][j]) {
                            dp[i + 1][j] = tmp;
                            from[i + 1][j] = 0;
                        }
                    }

                    if (j + 1 < M) {
                        int tmp = int(S[i][j + 1] != '.');
                        if (S[i][j] == S[i][j + 1])
                            tmp += dp[i][j];
                        if (tmp < dp[i][j + 1]) {
                            dp[i][j + 1] = tmp;
                            from[i][j + 1] = 1;
                        }
                    }
                }
        return dp[N - 1][M - 1];
    };

    for (answer = -1; step; step >>= 1) {
        answer += step;
        vector< vector<int> > dp(N, vector<int>(M, answer + 1));
        vector< vector<int> > from(N, vector<int>(M, 0));
        if (getDP(dp, from) <= answer)
            answer -= step;
    }

    ++answer;
    cout << answer << "\n";

    vector< vector<int> > dp(N, vector<int>(M, answer + 1));
    vector< vector<int> > from(N, vector<int>(M, 0));

    getDP(dp, from);
    vector< pair<int, int> > answer_way;
    int X = N - 1; int Y = M - 1;
    while (X >= 0 and Y >= 0) {
        answer_way.push_back({X, Y});
        if (from[X][Y] == 0)
            --X;
        else
            --Y;
    }

    reverse(answer_way.begin(), answer_way.end());
    for (auto p : answer_way)
        cout << p.first + 1 << " " << p.second + 1 << "\n";
}

