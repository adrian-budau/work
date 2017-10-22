#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;

int powmod(int x, int64_t y) {
    if (y == 0)
        return 1;
    if (y % 2)
        return (1LL * powmod(x, y - 1) * x) % kModulo;
    int v = powmod(x, y / 2);
    return (1LL * v * v) % kModulo;
}

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {

        int N, M; cin >> N >> M;

        int64_t V = 1LL * N * M;

        int64_t area = 5LL * V;
        int rows = 1, cols = 1;
        int64_t now = 1LL * N * M - 1;

        int64_t many = 0;
        while (rows != N || cols != M) {
            area += 2 * now;
            if (rows < N && cols < M && rows == cols)
                ++many;

            if (rows == N) {
                now -= rows;
                ++cols;
                many += rows;
                continue;
            }

            if (cols == M) {
                many += cols;
                now -= cols;
                ++rows;
                continue;
            }

            if (rows < cols) {
                many += cols;
                now -= cols;
                ++rows;
            } else {
                many += rows;
                now -= rows;
                ++cols;
            }
        }

        cout << "Case #" << test << ":\n";
        cout << area << " " << powmod(2, many) << "\n";
    }
}
