#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

int N, V[20], dp[1 << 20];

int answer(int state) {
    if (dp[state] != -1)
        return dp[state];
    int maxt = 0;
    dp[state] = 0;
    for (int i = 0; i < N; ++i)
        if ((1 << i) & state) {
            if (V[i] == maxt + 1)
                dp[state] += answer(state - (1 << i));
            maxt = V[i] > maxt ? V[i] : maxt;
        }

    return dp[state];
}

int main() {
    ifstream cin("invcs.in");
    ofstream cout("invcs.out");

    cin >> N;
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    memset(dp, -1, sizeof(dp));
    dp[0] = 1;
    cout << answer((1 << N) - 1) << "\n";
}
