#include <iostream>
#include <vector>

using namespace std;

static const int kMaxN = (2550 / 2) * (2500 / 2) + 10;

int main() {
    vector<int64_t> count(kMaxN, 0);
    for (int i = 1; i < kMaxN; ++i)
        for (int j = i; j < kMaxN; j += i)
            count[j]++;
    for (int i = 1; i < kMaxN; ++i)
        count[i] += count[i - 1];

    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        int64_t answer = 0;
        for (int i = 1; i < N; ++i)
            answer += count[i * (N - i) - 1];
        cout << answer << "\n";
    }
}
