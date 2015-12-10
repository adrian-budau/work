#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("spectacole.in");
    ofstream cout("spectacole.out");

    int N, M; cin >> N >> M;

    vector<int> A(N);
    vector<int> B(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i];
    for (int j = 0; j < N; ++j)
        cin >> B[j];

    vector<int> W(M), X(M), Y(M);
    int MAXT = 0;
    for (int i = 0; i < M; ++i) {
        cin >> W[i] >> X[i] >> Y[i];
        --W[i];
        MAXT = max(MAXT, Y[i]);
    }

    vector< vector< vector<int> > > shows(MAXT + 1, vector< vector<int> >(N));
    for (int i = 0; i < M; ++i)
        shows[X[i]][W[i]].push_back(Y[i]);

    vector< vector<int> > dp(MAXT + 1, vector<int>(N, 0));
    int answer = 0;
    for (int i = 0; i < MAXT; ++i)
        for (int p = 0; p < 2; ++p)
        for (int j = 0; j < N; ++j) {
            answer = max(answer, dp[i][j]);
            dp[i + 1][j] = max(dp[i + 1][j], dp[i][j]);
            for (int k = 0; k < N; ++k)
                if (k != j && i + A[j] + B[k] <= MAXT)
                    dp[i + A[j] + B[k]][k] = max(dp[i + A[j] + B[k]][k], dp[i][j]);
            for (auto &y : shows[i][j])
                dp[y][j] = max(dp[y][j], dp[i][j] + 1);
        }

    for (int j = 0; j < N; ++j)
        answer = max(answer, dp[MAXT][j]);
    cout << answer << "\n";
}
