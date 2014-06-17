#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

const int kModulo = 1000 * 1000 * 1000 + 7;
const int kMaxN = 1000007;

int pow(int A, int64_t B) {
    if (B == 0)
        return 1;
    if (B % 2)
        return int64_t(pow(A, B - 1)) * A % kModulo;
    int V = pow(A, B / 2);
    return int64_t(V) * V % kModulo;
}

inline int inverse(int A) {
    return pow(A, kModulo - 2);
}

int main() {
    ifstream cin("cabana.in");
    ofstream cout("cabana.out");

    int T; cin >> T;

    vector<int> fact(kMaxN);
    fact[0] = 1;
    for (int i = 1; i < kMaxN; ++i)
        fact[i] = int64_t(fact[i - 1]) * i % kModulo;

    while (T--) {
        int64_t N; int K;
        cin >> N >> K;

        cout << static_cast<int64_t>(pow(fact[K], N / K + 1)) * inverse(fact[K - N % K]) % kModulo << "\n";
    }
}
