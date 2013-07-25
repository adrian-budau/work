#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

const double kEpsilon = 1e-9;

int sign(int x1, int y1, int x2, int y2, int x3, int y3) {
    int area = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
    if (area > 0)
        return 1;
    if (area < 0)
        return -1;
    return 0;
}

int dist(int x1, int y1, int x2, int y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 -y2);
}

pair<int, int> angle(int x1, int y1, int x2, int y2, int x3, int y3) {
    int a = dist(x1, y1, x2, y2);
    int b = dist(x1, y1, x3, y3);
    int c = dist(x2, y2, x3, y3);

    int numerator = (a - b - c) * (a - b - c);
    if ((a - b - c) < 0)
        numerator = -numerator;
    int denominator = 2 * b * c;
    return make_pair(numerator, denominator);
}

int compare(pair<int, int> a, pair<int, int> b) {
    long long first = 1LL * a.first * b.second;
    long long second = 1LL * a.second * b.first;
    if (first < second)
        return -1;
    if (first > second)
        return 1;
    return 0;
}

int main() {
    int T; cin >> T;
    while (T--) {
        int N; cin >> N;

        vector<int> X(N), Y(N);
        for (int i = 0; i < N; ++i)
            cin >> X[i] >> Y[i];

        int bad = 0, total = 0;
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j)
                for (int k = j + 1; k < N; ++k)
                    for (int p = 0; p < N; ++p)
                        if (i != p and j != p and k != p) {
                            ++total;
                            if (sign(X[i], Y[i], X[j], Y[j], X[k], Y[k]) == 0)
                                continue;
                            pair<int, int> need = angle(X[i], Y[i], X[j], Y[j], X[k], Y[k]);
                            if (sign(X[i], Y[i], X[j], Y[j], X[k], Y[k]) != sign(X[i], Y[i], X[j], Y[j], X[p], Y[p]))
                                need = make_pair(-need.first, need.second);

                            pair<int, int> have = angle(X[i], Y[i], X[j], Y[j], X[p], Y[p]);
                            if (compare(have, need) >= 0)
                                ++bad;
                        }

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(10);
        cout << double(bad) / double(total) << "\n";
    }
}
