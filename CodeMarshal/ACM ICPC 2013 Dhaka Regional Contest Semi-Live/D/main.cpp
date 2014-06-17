#include <iostream>

using namespace std;

long long kModulo = 10000000000007LL;

int main() {
    int T; cin >> T;
    int test = 0;
    while (T--) {
        ++test;
        int L, M, N; cin >> L >> M >> N;

        L *= L;
        long long now = 1;
        for (int i = 0; i < M; ++i)
            now = (now * (L - i)) % kModulo;

        long long answer = 0;
        for (int i = M; i <= N; ++i) {
            answer = (answer + now) % kModulo;
            now = (now * (L - i)) % kModulo;
        }

        cout << "Case " << test << ": " << answer << "\n";
    }
}
