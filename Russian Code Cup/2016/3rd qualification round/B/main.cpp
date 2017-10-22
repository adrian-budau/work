#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    while (T--) {
        int N; cin >> N;

        string A, B; cin >> A >> B;
        int answer = 0;
        for (int i = 0; i + 1 < N; ++i)
            if (A[i] != B[i]) {
                A[i] ^= '0' ^ '1';
                A[i + 1] ^= '0' ^ '1';
                ++answer;
            }
        if (A[N - 1] != B[N - 1])
            cout << "-1\n";
        else
            cout << answer << "\n";
    }
}
