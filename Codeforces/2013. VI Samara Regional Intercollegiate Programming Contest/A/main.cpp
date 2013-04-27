#include <iostream>
#include <vector>

using namespace std;

int main() {
    int N, K; cin >> N >> K;

    if (N == 1) {
        cout << "0\n";
        return 0;
    }

    if (K == N) {
        cout << "-1\n";
        return 0;
    }

    vector<int> d(N + 1, 0x3f3f3f3f);
    d[1] = 0;

    for (int i = 2; i <= N; ++i) {
        for (int j = 1; j <= K; ++j)
            if (j < i && K - j <= N - i)
                d[i] = min(d[i], max(d[i - j] + 1, d[j]));
    }

    cout << d[N] << "\n";
}
