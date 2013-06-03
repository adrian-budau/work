#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>

using namespace std;

int main() {

    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N; cin >> N;

        vector<int> A(N), B(N);
        for (int i = 0; i < N; ++i)
            cin >> A[i];
        for (int j = 0; j < N; ++j)
            cin >> B[j];

        vector<int> answer, V(N);
        for (int i = 0; i < N; ++i)
            V[i] = i;
        answer = V;

        int steps = 0;
        do {
            if (++steps > 1000000)
                break;
            vector<int> newA(N), newB(N);
            for (int i = 0; i < N; ++i) {
                newA[i] = 1;
                for (int j = 0; j < i; ++j)
                    if (V[j] < V[i])
                        newA[i] = max(newA[i], newA[j] + 1);
            }

            for (int i = N - 1; i >= 0; --i) {
                newB[i] = 1;
                for (int j = i + 1; j < N; ++j)
                    if (V[j] < V[i])
                        newB[i] = max(newB[i], newB[j] + 1);
            }

            if (newA == A and newB == B) {
                answer = V;
                break;
            }
        } while (next_permutation(V.begin(), V.end()));
        cout << "Case #" << test << ": ";
        for (auto &x : answer)
            cout << x + 1 << " ";
        cout << "\n";
    }
}

