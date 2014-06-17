#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int solve(vector< vector<char> > A) {
    int N = A.size();
    int M = A[0].size();

    array<vector< vector<int> >, 2> dp;
    dp[0] = dp[1] = vector< vector<int> > (N, vector<int>(M, 0));
    for (int i = 0; i < M; ++i)
        if (A[0][i] == '#')
            break;
        else
            dp[0][0][i] = i + 1;

    for (int i = 1; i < N; ++i) {
        for (int k = 0; k < 2; ++k) {
            int bestSoFar = 0;
            for (int j = 0; j < M; ++j)
                if (A[i][j] == '#')
                    bestSoFar = 0;
                else {
                    bestSoFar = max(bestSoFar, dp[k][i - 1][j]);
                    if (bestSoFar > 0)
                        ++bestSoFar;
                    dp[k][i][j] = bestSoFar;
                }

            bestSoFar = 0;
            for (int j = M - 1; j >= 0; --j)
                if (A[i][j] == '#')
                    bestSoFar = 0;
                else {
                    bestSoFar = max(bestSoFar, dp[0][i - 1][j]);
                    if (bestSoFar > 0)
                        ++bestSoFar;
                    dp[1][i][j] = max(dp[1][i][j], bestSoFar); // we can with one left move here
                }
        }
    }

    int answer = 0;
    for (int k = 0; k < 2; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                answer = max(answer, dp[k][i][j]);
    return answer;
}

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N, M; cin >> N >> M;

        vector<string> S(N);
        for (int i = 0; i < N; ++i)
            cin >> S[i];

        vector< vector<char> > A(N, vector<char>(M)), B(M, vector<char>(N));

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                A[i][j] = B[j][i] = S[i][j];

        cout << "Case #" << test << ": " << max(solve(A), solve(B)) << "\n";
    }
}
