#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

double dist(pair<double, double> a, pair<double, double> b) {
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

static const int kMaxN = 17;

double dp[1 << (kMaxN - 1)][2 * (kMaxN - 1)];
double D[2 * kMaxN][2 * kMaxN];

inline double min(double a, double b) {
    return a < b ? a : b;
}

int main() {
    ifstream cin("seg.in");
    ofstream cout("seg.out");

    int T; cin >> T;
    while (T--) {
        int N; cin >> N;

        vector< pair<double, double> > points(2 * N);
        for (int i = 0; i < 2 * N; ++i)
            cin >> points[i].first >> points[i].second;

        if (N == 1) {
            cout << "0.000000\n";
            continue;
        }

        for (int i = 0; i < 2 * N; ++i)
            for (int j = 0; j < 2 * N; ++j)
                D[i][j] = dist(points[i], points[j]);

        for (int i = 0; i < (1 << (N - 1)); ++i)
            for (int j = 0; j < 2 * N - 2; ++j)
                dp[i][j] = 1e50;

        for (int i = 0; i < 2 * N; ++i) {
            int j = i - i % 2 + (1 - i % 2);
            dp[1 << (i / 2)][i] = D[2 * N - 2][j];
        }

        const int M = N - 1;
        for (int i = 1; i < (1 << M); ++i)
            for (int jj = 0; jj < M; ++jj)
                if ((1 << jj) & i)
                    for (int kk = 0; kk < M; ++kk)
                        if (!((1 << kk) & i))
                            for (int j = jj * 2; j < jj * 2 + 2; ++j)
                                for (int k = kk * 2; k < kk * 2 + 2; ++k) {
                                    int l = k ^ 1;
                                    dp[i | (1 << kk)][k] = min(dp[i | (1 << kk)][k], dp[i][j] + D[j][l]);
                                }
        double answer = 1e50;
        for (int j = 0; j < 2 * N - 2; ++j)
            answer = min(answer, dp[(1 << (N - 1)) - 1][j] + D[j][2 * N - 1]);

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(6);
        cout << answer << "\n";
    }
}
