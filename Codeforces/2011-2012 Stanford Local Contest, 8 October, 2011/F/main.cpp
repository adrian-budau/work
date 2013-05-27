#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits>

using namespace std;

int A[9][3] = {{1, 2, 3}, {4, 5, 6}, {3, 5, 7}, {7, 8, 9}, {10, 11, 12}, {6, 11, 13}, {13, 14, 15}, {9, 14, 16}, {16, 17, 18}};

vector< pair<int, int> > T[18];

int main() {
    for (int i = 0; i < 18; ++i)
        for (int j = 0; j < 9; ++j)
            if (A[j][0] == i + 1)
                T[i].push_back({A[j][1] - 1, A[j][2] - 1});
            else if (A[j][1] == i + 1)
                T[i].push_back({A[j][0] - 1, A[j][2] - 1});
            else if (A[j][2] == i + 1)
                T[i].push_back({A[j][0] - 1, A[j][1] - 1});

    vector<int> dp(1 << 18, 0);

    int total = 0;
    for (int i = (1 << 18) - 2; i >= 0; --i) {
        dp[i] = numeric_limits<int>::min();
        bool ok = true;
        for (int j = 0; j < 18; ++j)
            if ((1 << j) & i)
                continue;
            else {
                int newt = 0;
                for (auto &p : T[j])
                    if ((1 << p.first) & i)
                        if ((1 << p.second) & i)
                            ++newt;
                if (newt) {
                    dp[i] = max(dp[i], newt + dp[i + (1 << j)]);
                    ok = false;
                } else
                    dp[i] = max(dp[i], -dp[i + (1 << j)]);
            }
        if (ok)
            ++total;
    }

    cerr << total << "\n";
    int N;
    while (cin >> N) {
        if (N == 0)
            break;
        int state = 0;
        for (int i = 0; i < N; ++i) {
            int x; cin >> x; --x;
            state += (1 << x);
        }

        if (dp[state] == 0)
            cout << "Draw\n";
        else if (dp[state] < 0)
            cout << "Ralph wins\n";
        else
            cout << "Andy wins\n";
    }
}
