#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <array>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    array< vector<int>, 31> parts;
    for (int i = 1; i <= 30; ++i) {
        stable_partition(V.begin(), V.end(), [&](int x) {
            return (x & (1 << (i - 1))) == 0;
        });
        parts[i].reserve(N);
        for (auto &x : V)
            parts[i].push_back(x & ((1 << i) - 1));
    }

    int answer = 0;
    for (int t = 1; t <= 30; ++t) {
        int j = parts[t].size(); // overflow in 2^(t - 1)
        int k = parts[t].size(); // overflow in 2^t
        int l = parts[t].size(); // overflow in 2^t + 2^(t - 1)
        int count = 0;
        for (int i = 0; i < int(parts[t].size()); ++i) {
            while (l > 0 && parts[t][i] + parts[t][l - 1] >= (1 << t) + (1 << (t - 1)))
                --l;
            while (k > 0 && parts[t][i] + parts[t][k - 1] >= (1 << t))
                --k;
            while (j > 0 && parts[t][i] + parts[t][j - 1] >= (1 << (t - 1)))
                --j;
            if (k > i)
                count ^= ((k - max(i, j)) % 2);
            count ^= ((parts[t].size() - max(l, i)) % 2);
        }
        if (count)
            answer |= (1 << (t - 1));
    }

    cout << answer << "\n";
}
