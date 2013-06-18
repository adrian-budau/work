#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <cassert>
#include <fstream>
#include <set>

using namespace std;

int main() {
    ifstream input("input");
    ifstream output("output");

    int N, V, M;
    assert(input >> N >> V >> M);
    assert(1 <= N and N <= 300);
    assert(1 <= V and V <= 1000000000);
    assert(1 <= M and M <= 50000);

    vector<int> A(N), B(N);
    for (int i = 0; i < N; ++i) {
        assert(input >> A[i]);
        assert(0 <= A[i] and A[i] <= V);
    }

    int64_t sum = 0;
    for (int i = 0; i < N; ++i) {
        assert(input >> B[i]);
        assert(0 <= B[i] and B[i] <= V);
        sum += A[i] - B[i];
    }
    assert(sum == 0);

    set<pair<int, int>> E;
    for (int i = 0; i < M; ++i) {
        int x, y; assert(input >> x >> y);
        assert(1 <= x and x <= N);
        assert(1 <= y and y <= N);
        assert(x != y);
        E.insert({x, y});
        E.insert({y, x});
    }

    int answer;
    if (!(output >> answer)) {
        cout << "No output\n";
        return -1;
    }

    if (answer < 0 or answer > 2 * N * N) {
        cout << "Too many operations (or negative, unlikely)\n";
        return -1;
    }

    for (int i = 0; i < answer; ++i) {
        int x, y, d;
        if (!(output >> x >> y >> d)) {
            cout << "Missing line in output, line " << i + 2 << "\n";
            return -1;
        }

        if (x < 1 or x > N or y < 1 or y > N or d < 0 or d > V) {
            cout << "Wrong operation on line " << i + 2 << "\n";
            return -1;
        }

        --x; --y;
        if (d > A[x]) {
            cout << "Not enough water to transport on line " << i + 2 << "\n";
            return -1;
        }

        if (A[y] + d > V) {
            cout << "Overflow on line " << i + 2 << "\n";
            return -1;
        }

        A[x] -= d;
        A[y] += d;
    }

    if (A != B) {
        cout << "Not satisfied\n";
        return -1;
    }

    return 0;
}
