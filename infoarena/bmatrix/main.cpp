#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

int answer(vector< vector<int> > A) {
    int N = A.size();
    int M = A[0].size();
    vector< vector<int> > dp(N, vector<int>(M, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (A[i][j] == 0) {
                dp[i][j] = 1;
                if (i > 0) 
                    dp[i][j] += dp[i - 1][j];
            }

    vector<int> left(M, 0), right(M, 0);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            int height = N;
            for (int k = j; k < M; ++k) {
                height = min(height, dp[i][k]);
                left[k] = max(left[k], height * (k - j + 1));
                right[j] = max(right[j], height * (k - j + 1));
            }
        }

    for (int i = 1; i < M; ++i)
        left[i] = max(left[i], left[i - 1]);
    for (int i = M - 2; i >= 0; --i)
        right[i] = max(right[i], right[i + 1]);

    int answer = 0;
    for (int i = 1; i < M; ++i)
        answer = max(answer, left[i - 1] + right[i]);
    return answer;
}

int main() {
    ifstream cin("bmatrix.in");
    ofstream cout("bmatrix.out");

    int N, M; cin >> N >> M;
    vector<string> S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i];

    vector< vector<int> > A(N, vector<int>(M, 0)),
                          B(M, vector<int>(N, 0));

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            A[i][j] = B[j][i] = S[i][j] - '0';

    cout << max(answer(A), answer(B)) << "\n";
}
