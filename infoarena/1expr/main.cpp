#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const int kInfinite = 0x3f3f3f3f;

struct Info {
    int length, x, y;
};

string answer(int value, int priority, const vector< vector<Info> > &dp) {
    if (value == 1)
        return "1";

    if (priority == 4)
        return "(" + answer(value, 0, dp) + ")";

    if (dp[priority][value].x == -1)
        return answer(value, priority + 1, dp);

    if (priority == 0)
        return answer(dp[priority][value].x, 1, dp) + "+" + answer(dp[priority][value].y, 0, dp);
    if (priority == 1)
        return answer(dp[priority][value].x, 2, dp) + "*" + answer(dp[priority][value].y, 1, dp);
    if (priority == 2)
        return answer(dp[priority][value].x, 3, dp) + "^" + answer(dp[priority][value].y, 2, dp);

    return answer(dp[priority][value].x, 3, dp) + "!";
}

int main() {
    ifstream cin("1expr.in");
    ofstream cout("1expr.out");

    int T; cin >> T;
    vector<int> V(T);
    for (int i = 0; i < T; ++i)
        cin >> V[i];

    int N = *max_element(V.begin(), V.end());

    /*
     * 0 - Sum of factors
     * 1 - Product of powers
     * 2 - Powers of factorials
     * 3 - Factorial or Value
     * 4 - Value or Group
     */
    vector< vector<Info> > dp(5, vector<Info>(N + 1, {kInfinite, 0, 0}));
    vector<int> factorial{1, 1, 2, 6, 24, 120, 720, 5040};
    dp[4][1] = dp[3][1] = dp[2][1] = dp[1][1] = dp[0][1] = {1, -1, -1};

    for (int i = 2; i <= N; ++i) {
        // sum of j and i - j
        for (int j = 1; j < i; ++j)
            if (dp[1][j].length + 1 + dp[0][i - j].length < dp[0][i].length) {
                dp[0][i].length = dp[1][j].length + 1 + dp[0][i - j].length;
                dp[0][i].x = j;
                dp[0][i].y = i - j;
            }
        // product of j and i / j
        for (int j = 2; j * j <= N; ++j)
            if (i % j == 0)
                if (dp[2][j].length + 1 + dp[1][i / j].length < dp[1][i].length) {
                    dp[1][i].length = dp[2][j].length + 1 + dp[1][i / j].length;
                    dp[1][i].x = j;
                    dp[1][i].y = i / j;
                }
        // power of factorials j ^ k
        for (int j = 2; j * j <= N; ++j)
            if (i % j == 0) {
                int k = 0;
                int aux = i;
                while (aux % j == 0) {
                    aux /= j;
                    ++k;
                }
                if (aux != 1)
                    continue;
                if (dp[3][j].length + 1 + dp[2][k].length < dp[2][i].length) {
                    dp[2][i].length = dp[3][j].length + 1 + dp[2][k].length;
                    dp[2][i].x = j;
                    dp[2][i].y = k;
                }
            }

        // factorial
        for (int k = 2; k <= 7; ++k)
            if (factorial[k] == i)
                if (dp[3][k].length + 1 < dp[3][i].length) {
                    dp[3][i].length = dp[3][k].length + 1;
                    dp[3][i].x = k;
                    dp[3][i].y = -1;
                }

        // value
        for (int j = 2; j >= 0; --j)
            if (dp[j + 1][i].length < dp[j][i].length) {
                dp[j][i].length = dp[j + 1][i].length;
                dp[j][i].x = -1;
                dp[j][i].y = -1;
            }
        dp[4][i].length = dp[0][i].length + 2;
        dp[4][i].x = dp[4][i].y = -1;
        for (int j = 3; j >= 0; --j)
            if (dp[j + 1][i].length < dp[j][i].length) {
                dp[j][i].length = dp[j + 1][i].length;
                dp[j][i].x = dp[j][i].y = -1;
            }
    }

    for (auto &x : V) {
        cout << answer(x, 0, dp) << "\n";
    }
}

