#include <iostream>

using namespace std;

const int kModulo = 1000 * 1000 * 1000 + 7;

int pow(int a, int b) {
    if (b == 0)
        return 1;
    if (b % 2)
        return 1LL * pow(a, b - 1) * a % kModulo;

    int v = pow(a, b / 2);
    return 1LL * v * v % kModulo;
}

int main() {
    int T; cin >> T;

    while (T--) {
        int N, M; cin >> N >> M;

        if (N == 1) {
            cout << M << "\n";
            continue;
        }

        int answer = 1LL * M * (M - 1) % kModulo;

        answer = 1LL * answer * pow(M - 2, N - 2) % kModulo;
        cout << answer << "\n";
    }
}
