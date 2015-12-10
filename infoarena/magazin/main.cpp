#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

static const int kInfinite = numeric_limits<int>::max() / 2;

/**
 * state[i][j] =
 *  - i = where am i, 0 down, 1 up
 *  - j = 0 if the other side is not touched, 1 if it is but not connected, 2 if it is and connected
 */
typedef array< array<int, 3>, 2> state;

int main() {
    ifstream cin("magazin.in");
    ofstream cout("magazin.out");

    int P, N, M, D; cin >> P >> N >> M >> D;
    ++M;

    vector< vector<int> > V(N);
    for (int i = 0; i < P; ++i) {
        int x, y; cin >> x >> y;
        V[x - 1].push_back(y);
    }

    vector<int> bestNoTouch(N, 0);
    array< vector<int>, 2> bestFromDirection;
    bestFromDirection[0] = bestFromDirection[1] = vector<int>(N, 0);
    for (int i = 0; i < N; ++i) {
        if (V[i].empty())
            continue;
        sort(V[i].begin(), V[i].end());
        bestNoTouch[i] = min(2 * V[i].back(), 2 * (M - V[i].front()));
        for (int j = 0; j + 1 < int(V[i].size()); ++j)
            bestNoTouch[i] = min(bestNoTouch[i], 2 * V[i][j] + 2 * (M - V[i][j + 1]));
        bestFromDirection[0][i] = 2 * V[i].back();
        bestFromDirection[1][i] = 2 * (M - V[i].front());
    }

    vector<state> dp(N, state({}));
    dp[0][0][0] = bestFromDirection[0][0];
    dp[0][0][1] = bestNoTouch[0];
    dp[0][0][2] = 2 * M;
    dp[0][1][0] = kInfinite;
    dp[0][1][1] = kInfinite;
    dp[0][1][2] = M;

    for (int i = 1; i < N; ++i)
        for (int j = 0; j < 2; ++j) {
            dp[i][j][0] = min({
                dp[i - 1][j][0] + D + bestFromDirection[j][i],
                dp[i - 1][j][2] + D + bestFromDirection[j][i],
            });

            dp[i][j][1] = min({
                dp[i - 1][j][0] + D + bestNoTouch[i],
                dp[i - 1][j][1] + 3 * D + bestNoTouch[i],
                dp[i - 1][j][2] + D + bestNoTouch[i],
            });

            dp[i][j][2] = min({
                dp[i - 1][j][0] + D + 2 * M,
                dp[i - 1][j][1] + 3 * D + 2 * M,
                dp[i - 1][j][2] + 3 * D + bestNoTouch[i],
                dp[i - 1][j][2] + D + 2 * M,
                dp[i - 1][1 - j][0] + D + M,
                dp[i - 1][1 - j][1] + 3 * D + M,
                dp[i - 1][1 - j][2] + D + M
            });
        }
    cout << min(dp.back()[0][0], dp.back()[0][2]) << "\n";
}
