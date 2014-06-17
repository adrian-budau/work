#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("marsmusic.in");
    ofstream cout("marsmusic.out");

    int N, M; cin >> N >> M;
    vector<int> V(M);

    for (int i = 0; i < M; ++i)
        cin >> V[i];

    vector< vector<double> > dp(M + 1, vector<double>(N + 1, 0.0));
    dp[0][0] = 1.0;
    for (int i = 0; i < M; ++i) {
        vector< vector<double> > future(M + 1, vector<double>(N + 1, 0.0));
        for (int j = 0; j <= i + 1; ++j)
            for (int k = 0; k <= N; ++k) {
                future[j][k] = dp[j][k] * double(i + 1 - j) / double(i + 2);
                if (j > 0 and k >= V[i])
                    future[j][k] += dp[j - 1][k - V[i]] * double(j) / double(i + 2);
            }
        swap(dp, future);
    }

    double answer = 0;
    for (int i = 0; i < M; ++i) {
        vector< vector<double> > past(M, vector<double>(N + 1, 0.0));
        for (int j = 0; j < M; ++j)
            for (int k = 0; k <= N; ++k) {
                past[j][k] = dp[j][k];
                if (j > 0 and k >= V[i])
                    past[j][k] -= past[j - 1][k - V[i]] * double(j) / double(M + 1);
                past[j][k] = past[j][k] * double(M + 1) / double(M - j);
            }

        vector<double> chance(N + 1, 0.0);
        for (int j = 0; j < M; ++j)
            for (int k = 0; k <= N - V[i]; ++k) {
                chance[k + 1] += past[j][k];
                if (k + V[i] + 1 <= N)
                    chance[k + V[i] + 1] -= past[j][k];
            }

        double now = 0;
        for (int j = 0; j <= N; ++j) {
            now += chance[j];
            answer += now * now;
        }
    }

    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(9);

    cout << answer << "\n";
}
