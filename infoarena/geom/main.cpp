#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct Point {
    int x, y;
    long long distance;

    bool operator<(const Point& that) const {
        return distance < that.distance;
    }

    void getDistance(const Point& A, const Point& B) {
        distance = 1LL * x * (A.y - B.y) + 1LL * A.x * (B.y - y) + 1LL * B.x * (y - A.y);
    }

    long long value() const {
        return 1LL * x * x + 1LL * y * y;
    }

    Point operator-(const Point& that) const {
        return {x - that.x, y - that.y, 0};
    }
};

int main() {
    ifstream cin("geom.in");
    ofstream cout("geom.out");

    int N, K; cin >> N >> K;
    vector<Point> P(N);
    for (int i = 0; i < N; ++i)
        cin >> P[i].x >> P[i].y;

    double answer = numeric_limits<double>::max();
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) {
            vector<Point> A, B;
            for (int k = 0; k < N; ++k) {
                Point now(P[k]);
                now.getDistance(P[i], P[j]);

                if (now.distance <= 0)
                    A.push_back(now);
                if (now.distance >= 0)
                    B.push_back(now);
            }

            for (auto &p : A)
                p.distance = -p.distance;

            double divide = sqrt((P[i] - P[j]).value());

            if (int(A.size()) >= K) {
                nth_element(A.begin(), A.begin() + K - 1, A.end());
                answer = min(answer, A[K - 1].distance / divide);
            }

            if (int(B.size()) >= K) {
                nth_element(B.begin(), B.begin() + K - 1, B.end());
                answer = min(answer, B[K - 1].distance / divide);
            }
        }

    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(10);
    cout << answer << "\n";
}
