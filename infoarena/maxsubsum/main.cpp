#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cassert>

using namespace std;

vector<int64_t> solve(vector<int> A) {
    int N = A.size();
    vector<int64_t> best(N + 1, numeric_limits<int64_t>::min());
    best[0] = 0;
    for (int i = 0; i < N; ++i) {
        int64_t aux = 0;
        for (int j = i; j < N; ++j) {
            aux += A[j];
            best[j - i + 1] = max(best[j - i + 1], aux);
        }
    }
    return best;
}

int main() {
    ifstream cin("maxsubsum.in");
    ofstream cout("maxsubsum.out");

    int N, M; assert(cin >> N >> M);
    assert(1 <= N && N <= 2000);
    assert(1 <= M && M <= 2000);
    vector<int> A(N), B(M);
    for (int i = 0; i < N; ++i) {
        assert(cin >> A[i]);
        assert(-1000 * 1000 * 1000 <= A[i] && A[i] <= 1000 * 1000 * 1000);
    }

    for (int j = 0; j < M; ++j) {
        assert(cin >> B[j]);
        assert(-1000 * 1000 * 1000 <= B[j] && B[j] <= 1000 * 1000 * 1000);
    }

    auto bestA = solve(A), bestB = solve(B);
    int64_t answer = 0;
    for (int i = 0; i <= N; ++i)
        for (int j = 0; j <= M; ++j)
            answer = max(answer, bestA[i] * j  + bestB[j] * i);

    cout << answer << "\n";
}
