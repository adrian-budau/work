#include <iostream>
#include <vector>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 9;
static const int kMaxN = 100 * 1000 + 5;

int pow(int A, int B) {
    if (B == 0)
        return 1;
    if (B % 2)
        return 1LL * A * pow(A, B - 1) % kModulo;

    int v = pow(A, B / 2);
    return 1LL * v * v % kModulo;
}

int main() {
    int T; cin >> T;
    vector<int> fact(kMaxN), ifact(kMaxN);
    fact[0] = ifact[0] = 1;
    for (int i = 1; i < kMaxN; ++i) {
        fact[i] = 1LL * fact[i - 1] * i % kModulo;
        ifact[i] = pow(fact[i], kModulo - 2);
    }

    auto comb = [&](int N, int K) {
        int answer = fact[N];
        answer = 1LL * answer * ifact[K] % kModulo;
        answer = 1LL * answer * ifact[N - K] % kModulo;
        return answer;
    };

    while (T--) {
        int N, Q; cin >> N >> Q;
        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];

        while (Q--) {
            int M; cin >> M;
            vector<int> count(M, 0);
            for (auto value : V) {
                value %= M;
                if (value < 0)
                    value += M;
                count[value]++;
            }

            vector< vector<int> > many(M, vector<int>(M, 0));
            for (int i = 0; i < M; ++i)
                for (int j = 1; j <= count[i]; ++j)
                    many[i][i * j % M] = (many[i][i * j % M] + comb(count[i], j)) % kModulo;

            vector<int> present(M, 0);
            present[0] = 1;
            for (int i = 0; i < M; ++i) {
                vector<int> future = present;
                for (int j = 0; j < M; ++j)
                    if (many[i][j])
                        for (int k = 0; k < M; ++k) {
                            future[(k + j) % M] = (future[(k + j) % M] + 1LL * present[k] * many[i][j]) % kModulo;
                        }
                swap(future, present);
            }
            cout << present[0] << "\n";
        }
    }
}
