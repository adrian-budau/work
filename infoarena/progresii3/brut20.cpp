#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 9;

int main() {
    ifstream cin("progresii3.in");
    ofstream cout("progresii3.out");

    int64_t V; int N, K; cin >> V >> N >> K;

    assert(V <= 100 * 1000);
    assert(N <= 50);
    assert(K <= 30);

    vector<int64_t> A(K);
    for (int i = 0; i < K; ++i)
        cin >> A[i];

    // V - (N - 1) + V - 2 * (N - 1) + ... + V - P * (N - 1)
    // V * P - (N - 1) * P * (P + 1) / 2
    int64_t P = (V / (N - 1)) % kModulo;
    int answer = ((V % kModulo) * P - (N - 1) * ((P * (P + 1) / 2) % kModulo)) % kModulo;
    answer %= kModulo;

    for (int ratio = 1; ratio <= V / (N - 1); ++ratio) {
        set<int64_t> beginnings;
        for (auto &x : A)
            for (int j = 1; j <= N; ++j) {
                int64_t start = x - int64_t(j - 1) * ratio;
                int64_t end = x + int64_t(N - j) * ratio;
                if (start >= 1 && end <= V)
                    beginnings.insert(start);
            }
        answer = (answer - int(beginnings.size())) % kModulo;
    }

    if (answer < 0)
        answer += kModulo;

    cout << answer << "\n";
}
