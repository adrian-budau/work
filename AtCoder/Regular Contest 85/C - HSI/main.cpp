#include <iostream>

using namespace std;

int main() {
    int N, M; cin >> N >> M;

    int answer = 1900 * M + 100 * (N - M);
    cout << answer * ((1 << M)) << "\n";
}
