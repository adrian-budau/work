#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Point {
  public:
    bool operator<(const Point& that) const {
        return angle < that.angle;
    }

    int x, y;
    double angle;
};

istream& operator>>(istream& stream, Point& object) {
    stream >> object.x >> object.y;
    return stream;
}

const double eps = 1e-9;

int cmp(double a, double b) {
    if (a + eps < b)
        return -1;
    if (b + eps < a)
        return 1;
    return 0;
}

int main() {
    int N; cin >> N;

    vector<Point> P(N);
    for (int i = 0; i < N; ++i)
        cin >> P[i];

    long long answer = 0;
    for (int i = 0; i < N; ++i) {
        vector<Point> C(P.begin() + i + 1, P.end());
        C.insert(C.end(), P.begin(), P.begin() + i);
        for (int j = 0; j < int(C.size()); ++j)
            C[j].angle = atan2(C[j].y - P[i].y, C[j].x - P[i].x);

        sort(C.begin(), C.end());
        double pi = acos(-1);

        int M = C.size();
        C.resize(C.size() * 2);
        for (int j = 0; j < M; ++j) {
            C[j + M] = C[j];
            C[j + M].angle = C[j].angle + 2 * pi;
        }

        int left = 0, current = 0;;
        for (int j = 0; j < M; ++j) {
            while (left < j + M and cmp(C[j].angle + pi, C[left].angle) > 0)
                ++left;
            while (current < j + M and cmp(C[j].angle, C[current].angle) >= 0)
                ++current;

            answer += 1LL * (left - current) * (P[i].x * C[j].y - P[i].y * C[j].x);
        }

    }

    cout << answer / 2 << "." << (answer % 2) * 5 << "\n";
}
