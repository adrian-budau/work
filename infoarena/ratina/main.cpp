#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("ratina.in");
    ofstream cout("ratina.out");

    int N, M; cin >> N >> M;

    vector<string> S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i];

    vector<int> P(N);
    for (int i = 0; i < N; ++i)
        P[i] = i;
    sort(P.begin(), P.end(), [&](int x, int y) {
        return S[x] < S[y];
    });

    vector<int> pos(N);
    for (int i = 0; i < N; ++i)
        pos[P[i]] = i;

    vector<int> diff(N - 1);
    for (int i = 0; i + 1 < N; ++i)
        for (diff[i] = 0; diff[i] < int(S[P[i]].size()) && diff[i] < int(S[P[i + 1]].size()) && S[P[i]][diff[i]] == S[P[i + 1]][diff[i]]; ++diff[i]);

    int log;
    for (log = 1; (1 << log) < N; ++log);
    vector< vector<int> > rmq(log, vector<int>(N - 1, 0));
    rmq[0] = diff;
    for (int i = 1; i < log; ++i)
        for (int j = 0; j + (1 << (i - 1)) < N - 1; ++j)
            rmq[i][j] = min(rmq[i - 1][j], rmq[i - 1][j + (1 << (i - 1))]);

    vector<int> logv(N);
    logv[1] = 0;
    for (int i = 2; i < N; ++i)
        logv[i] = 1 + logv[i / 2];
    auto query = [&](int x, int y) {
        int diff = y - x + 1;
        int log = logv[diff];
        return min(rmq[log][x], rmq[log][y - (1 << log) + 1]);
    };

    for (int i = 0; i < M; ++i) {
        int K; cin >> K;
        int mint = N, maxt = 0;
        for (int j = 0; j < K; ++j) {
            int x; cin >> x;
            --x;
            mint = min(mint, pos[x]);
            maxt = max(maxt, pos[x]);
        }

        if (mint == maxt) {
            cout << S[P[mint]].size() << "\n";
            continue;
        }

        cout << query(mint, maxt - 1) << "\n";
    }
}

