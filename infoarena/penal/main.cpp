#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("penal.in");
    ofstream cout("penal.out");

    int N, M, P; cin >> N >> M >> P;

    vector< pair<int, int> > A(P);
    for (int i = 0; i < P; ++i)
        cin >> A[i].first >> A[i].second;

    for (int i = 0; i < M; ++i) {
        for (auto &p : A) {
            if (N % 2 && p.first == N / 2 + 1 && p.second == N / 2 + 1) {
                p = make_pair(N, N);
                continue;
            }

            int iteration = min({p.first, p.second, N - p.first + 1, N - p.second + 1});
            // iteration 1 has N * 4 - 4
            // iteration 2 has (N - 2) * 4 - 4
            // ...
            // iteration K has (N - 2 * (K - 1)) * 4 - 4
            // total sum = 4 * N * K - 2 * (0 + 1 + 2 + ... + K - 1) * 4 - 4 * K
            //           = 4 * N * K - (K - 1) * K * 4 - 4 * K
            //           = 4 * K * (N - 1 - K + 1)
            //           = 4 * K * (N - K)
            int total = 4 * (iteration - 1) * (N - iteration + 1);
            if (p.first == iteration)
                total += p.second - iteration;
            else if (N - p.second + 1 == iteration)
                total += p.first - iteration + (N - 2 * iteration + 1);
            else if (N - p.first + 1 == iteration)
                total += N - iteration + 1 - p.second + (N - 2 * iteration + 1) * 2;
            else
                total += N - iteration + 1 - p.first + (N - 2 * iteration + 1) * 3;
            int row = total / N + 1;
            int col = total % N + 1;
            p = make_pair(row, col);
        }

        for (auto p : A)
            cout << p.first << " " << p.second << " ";
        cout << "\n";
    }
}
