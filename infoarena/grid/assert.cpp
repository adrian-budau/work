#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("grid.in");
    ofstream cout("grid.out");

    int N, K; cin >> N >> K;

    assert(1 <= N && N <= 10 * 1000);
    assert(1 <= K && K <= 150 * 1000);

    int A[3] = {N, N, N};

    for (int i = 0; i < K; ++i) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;

        assert(0 <= x1 && x1 <= 2);
        assert(0 <= x2 && x2 <= 2);
        assert(0 <= y1 && y1 < A[x1]);
        --A[x1];
        assert(0 <= y2 && y2 <= A[x2]);
        ++A[x2];
    }
}
