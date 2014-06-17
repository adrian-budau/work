#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        vector<int> A(N), B(N);
        for (int i = 0; i < N; ++i)
            cin >> A[i];
        for (int j = 0; j < N; ++j)
            cin >> B[j];

        if (N == 1) {
            cout << max(-1, A[0] - B[0]) << "\n";
            continue;
        }

        if (N == 2) {
            if (A[0] + A[1] != B[0] + B[1])
                cout << "-1\n";
            else
                cout << abs(A[0] - B[0]) << "\n";
            continue;
        }

        int begin = accumulate(A.begin(), A.end(), 0),
            end   = accumulate(B.begin(), B.end(), 0);

        if (end < begin || (end - begin) % (N - 2)) {
            cout << "-1\n";
            continue;
        }

        int steps = (end - begin) / (N - 2);
        for (auto &x : A)
            x += steps;

        int answer = steps;
        bool bad = false;
        for (int i = 0; i < N; ++i) {
            if (A[i] < B[i] || (A[i] - B[i]) % 2)
                bad = true;
            steps -= (A[i] - B[i]) / 2;
        }

        if (steps < 0)
            bad = true;

        if (bad) {
            cout << "-1\n";
            continue;
        }

        cout << answer << "\n";
    }
}
