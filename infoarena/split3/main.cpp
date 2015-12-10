#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

static const double kEps = 1e-5;

int sign(double x) {
    if (x < -kEps)
        return -1;
    if (x > kEps)
        return 1;
    return 0;
}

struct Point {
    Point(double _x = 0, double _y = 0):
            x(_x),
            y(_y) {
    }

    bool operator==(const Point& that) const {
        return sign(x - that.x) == 0 && sign(y - that.y) == 0;
    }

    bool operator!=(const Point& that) const {
        return sign(x - that.x) != 0 || sign(y - that.y) != 0;
    }

    Point operator-(const Point& that) const {
        return Point(x - that.x, y - that.y);
    }

    Point operator+(const Point& that) const {
        return Point(x + that.x, y + that.y);
    }

    double x, y;
};

Point operator*(const double& alpha, const Point& that) {
    return Point(alpha * that.x, alpha * that.y);
}

double area(Point A, Point B, Point C) {
    return A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y);
}

void printLine(ostream& cout, Point A, Point B) {
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(9);
    cout << A.y - B.y << " " << B.x - A.x << " " << A.x * B.y - A.y * B.x << "\n";
}

Point splitHalf(vector<Point> polygon, Point from) {
    for (int i = 0; i + 1 < int(polygon.size()); ++i)
        if (sign(area(polygon[i], from, polygon[i + 1])) == 0 && from != polygon[i + 1]) {
            rotate(polygon.begin(), polygon.begin() + i + 1, polygon.end());
            break;
        }

    double totalArea = 0;
    for (int i = 0; i + 1 < int(polygon.size()); ++i)
        totalArea += fabs(area(from, polygon[i], polygon[i + 1]));

    totalArea /= 2;

    double currentArea = 0;
    for (int i = 0; i + 1 < int(polygon.size()); ++i) {
        double toAdd = fabs(area(from, polygon[i], polygon[i + 1])) / 2;
        double nextArea = currentArea + toAdd;
        if (nextArea >= totalArea / 2) { // gotcha, lets throw the Master Ball to catch this Pokemon... ahem Point
            double needed = totalArea / 2 - currentArea;
            double ratio = needed / toAdd;
            Point to = polygon[i] + ratio * (polygon[i + 1] - polygon[i]);
            return to;
        }

        currentArea = nextArea;
    }

    throw new logic_error("We couldn't divide the polygon in 2, wth?");
}

pair< vector<Point>, vector<Point> > divide(vector<Point> polygon, Point P, Point Q) {
    vector<Point> first, second;
    for (int i = 0; i < int(polygon.size()); ++i) {
        Point now = polygon[i], next = polygon[(i + 1) % polygon.size()];
        int currentSign = sign(area(P, Q, now));
        int nextSign = sign(area(P, Q, next));
        bool isWithP = sign(area(now, next, P)) == 0 && P != next;
        bool isWithQ = sign(area(now, next, Q)) == 0 && Q != next;

        if (currentSign < 0)
            first.push_back(now);
        if (currentSign > 0)
            second.push_back(now);

        if (currentSign != nextSign) {
            if (isWithP) {
                first.push_back(P);
                second.push_back(P);
            } else if (isWithQ) {
                first.push_back(Q);
                second.push_back(Q);
            }
        }
    }

    return make_pair(first, second);
}

int main() {
    ifstream cin("split3.in");
    ofstream cout("split3.out");

    int N; cin >> N;
    vector<Point> P(N);
    for (int i = 0; i < N; ++i)
        cin >> P[i].x >> P[i].y;

    Point Q;
    cin >> Q.x >> Q.y;

    auto halfPoint = splitHalf(P, Q);

    vector<Point> first, second;
    tie(first, second) = divide(P, Q, halfPoint);

    double from = 0, to = 1;
    double answer = 0.5;
    {
        auto otherPointFirst = splitHalf(first, Q);
        auto otherPointSecond = splitHalf(second, Q);

        int currentSign = sign(area(otherPointFirst, otherPointSecond, Q));
        if (currentSign == -1)
            swap(Q, halfPoint);
    }

    do {
        double mid = (from + to) / 2;
        Point now = Q + mid * (halfPoint - Q);

        auto otherPointFirst = splitHalf(first, now);
        auto otherPointSecond = splitHalf(second, now);

        int currentSign = sign(area(otherPointFirst, otherPointSecond, now));
        if (currentSign == 1)
            from = mid;
        else if (currentSign == -1)
            to = mid;
        else {
            answer = mid;
            break;
        }
    } while (true);

    Point now = Q + answer * (halfPoint - Q);
    Point other = splitHalf(first, now);
    printLine(cout, Q, halfPoint);
    printLine(cout, now, other);
}
