#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

using namespace std;

bool between(int x1, int x2, int x3) {
    if (x1 <= x3 and x3 <= x2)
        return true;
    if (x2 <= x3 and x3 <= x1)
        return true;
    return false;
}

bool in(int x1, int y1, int x2, int y2, int x3, int y3) {
    int area = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    if (area > 0)
        return false;
    if (area < 0)
        return true;
    if (area == 0) {
        if (between(x1, x2, x3) and between(y1, y2, y3)) {
            if (x1 != x3 or y1 != y3)
                if (x2 != x3 or y2 != y3)
                    return true;
        }
    }
    return false;
}

typedef double double64;

int main() {
    int N; cin >> N;

    double64 answer = 0;
    vector<double64> powers((N + 1) * (N + 1));
    powers[0] = 1;
    for (int i = 1; i < int(powers.size()); ++i) {
        powers[i] = powers[i - 1] * N / double64(N + 1);
    }
    vector<double64> answers(powers.size(), 0);

    int64_t total = 1LL * (N + 1) * (N + 1) * ((N + 1) * (N + 1) - 1);
    int64_t count = 0;
    int part = 1;
    int64_t next = part * total / 100;
    for (int i = 0; i <= N; ++i)
        for (int j = 0; j <= N; ++j)
            for (int k = 0; k <= N; ++k)
                for (int p = 0; p <= N; ++p)
                    if (i != k || j != p) {
                        int many = 0;
                        for (int l = 0; l <= N; ++l) {
                            if (i == l and k == l) {
                                many += abs(p - j) - 1;
                                continue;
                            }

                            bool leftIn = in(i, j, k, p, l, 0);
                            bool rightIn = in(i, j, k, p, l, N);
                            if (!leftIn && !rightIn)
                                continue;
                            if (leftIn and rightIn) {
                                many += N + 1;
                                continue;
                            }

                            // x1 + (x2 - x1) * t = x3 = l
                            // y1 + (y2 - y1) * t = y3

                            int maybeintersect = (l - i) * (p - j);
                            int x3 = maybeintersect / (k - i) + j;
                            if (maybeintersect * (k - i) < 0 and maybeintersect % (k - i))
                                --x3;
                            if (maybeintersect % (k - i) and x3 < N)
                                ++x3;

                            if (leftIn) {
                                if (!in(i, j, k, p, l, x3))
                                    --x3;
                                many += x3 + 1;
                                continue;
                            }

                            if (!in(i, j, k, p, l, x3))
                                ++x3;
                            many += N - x3 + 1;
                        }
                        assert(many <= int(powers.size()));
                        answers[many] += (i * p - k * j) / 2.0l;
                        if (++count == next) {
                            cout << ".";
                            cout.flush();
                            ++part;
                            next = part * total / 100;
                        }
                    }
    for (int i = answers.size() - 1; i > 0; --i) {
        answers[i - 1] += answers[i] * powers[1];
    }
    answer = answers[0] / (N + 1) / (N + 1);
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(27);
    cout << answer << "\n";
}
