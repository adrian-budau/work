#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <array>
#include <limits>

using namespace std;

const int kInfinite = numeric_limits<int>::max();

int main() {
    ifstream cin("telegraf.in");
    ofstream cout("telegraf.out");

    vector<int> frequency(36);
    for (int i = 0; i < 36; ++i)
        cin >> frequency[i];

    sort(frequency.begin(), frequency.end());
    for (auto it = frequency.begin(); it != frequency.end(); ++it)
        if (*it) {
            frequency = vector<int>(it, frequency.end());
            break;
        }

    int N = frequency.size();
    frequency.insert(frequency.begin(), 0);
    for (int i = 1; i <= N; ++i)
        frequency[i] += frequency[i - 1];

    vector< vector< vector<int> > > dp(N + 1, vector< vector<int> >(N + 1, vector<int>(N + 1, kInfinite)));
    dp[N][1][0] = 0;
    for (int i = N; i >= 0; --i)
        for (int j = 0; j <= N; ++j)
            for (int k = 0; k <= N; ++k) {
                if (dp[i][j][k] == kInfinite)
                    continue;
                if (j == 0 and k == 0)
                    continue;
                for (int p = 0; p <= i and p <= j + k; ++p)
                    if (j + k - p <= N)
                        dp[i - p][j + k - p][j] = min(dp[i - p][j + k - p][j], dp[i][j][k] + frequency[i]);
            }

    cout << dp[0][0][0] << "\n";
}
