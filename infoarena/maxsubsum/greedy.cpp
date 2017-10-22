#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

pair<int64_t, int> solve(vector<int> A) {
    int N = A.size();
    pair<int64_t, int> best;
    for (int i = 0; i < N; ++i) {
        int64_t aux = 0;
        for (int j = i; j < N; ++j) {
            aux += A[j];
            best = max(best, make_pair(aux, j - i + 1));
        }
    }
    return best;
}

int main() {
    ifstream cin("maxsubsum.in");
    ofstream cout("maxsubsum.out");

    int N, M; cin >> N >> M;
    vector<int> A(N), B(M);
    for (int i = 0; i < N; ++i)
        cin >> A[i];
    for (int j = 0; j < M; ++j)
        cin >> B[j];

    auto bestA = solve(A), bestB = solve(B);
    int64_t answer = bestA.first * bestB.second + bestB.first * bestA.second;

    cout << answer << "\n";
}
