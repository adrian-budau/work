#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

double area(pair<double, double> A, pair<double, double> B, pair<double, double> C) {
    return A.first * (B.second - C.second) + B.first * (C.second - A.second) + C.first * (A.second - B.second);
}

const double eps = 0.0002;

int cmp(double a, double b) {
    if (a + eps < b)
        return -1;
    if (b + eps < a)
        return 1;
    return 0;
}

struct Line {
    Line(double _A = 0, double _B = 0, double _C = 0):
            A(_A), B(_B), C(_C) {
    }

    double calculate(double X, double Y) const {
        return A * X + B * Y + C;
    }

    double calculate(pair<double, double> P) const {
        return calculate(P.first, P.second);
    }

    void simplify() {
        if (cmp(A, 0) < 0 or (cmp(A, 0) == 0 and cmp(B, 0) < 0) or (cmp(A, 0) == 0 and cmp(B, 0) == 0 and cmp(C, 0) < 0)) {
            A *= -1;
            B *= -1;
            C *= -1;
        }

        if (cmp(C, 0)) {
            A /= C;
            B /= C;
            C = 1;
        } else if (cmp(B, 0)) {
            A /= B;
            B = 1;
        } else if (cmp(A, 0)) {
            A = 1;
        }
    }

    bool symmetricalPoints(pair<double, double> P1, pair<double, double> P2) const {
        double X1 = P1.first, Y1 = P1.second, X2 = P2.first, Y2 = P2.second;
        if (cmp(calculate(X1, Y1) + calculate(X2, Y2), 0))
            return false;

        double left = calculate(X1, Y1) * 2.0;
        left /= sqrt(A * A + B * B);
        double right = sqrt((X1 - X2) * (X1 - X2) + (Y1 - Y2) * (Y1 - Y2));

        return cmp(fabs(left), right) == 0;
    }

    static Line perpendicularBisector(pair<double, double> P1, pair<double, double> P2) {
        double X1 = P1.first, Y1 = P1.second, X2 = P2.first, Y2 = P2.second;
        return {2 * (X2 - X1), 2 * (Y2 - Y1), X1 * X1 - X2 * X2 + Y1 * Y1 - Y2 * Y2};
    }

    static Line fromPoints(pair<double, double> P1, pair<double, double> P2) {
        double X1 = P1.first, Y1 = P1.second, X2 = P2.first, Y2 = P2.second;
        return {Y2 - Y1, X1 - X2, X2 * Y1 - X1 * Y2};
    }

    bool operator<(const Line& that) const {
        if (cmp(A, that.A))
            return A < that.A;
        if (cmp(B, that.B))
            return B < that.B;
        return cmp(C, that.C) < 0;
    }

    bool operator==(const Line& that) const {
        return cmp(A, that.A) == 0 and cmp(B, that.B) == 0 and cmp(C, that.C) == 0;
    }

    double A, B, C;
};

int main() {
    ifstream cin("dmg.in");
    ofstream cout("dmg.out");

    int N; cin >> N;
    
    vector< pair<double, double> > P(N);
    for (int i = 0; i < N; ++i)
        cin >> P[i].first >> P[i].second;

    sort(P.begin(), P.end());
    vector< vector< pair<double, double> > > polygons;

    while (P.size()) {
        if (P.size() <= 2) {
            polygons.push_back(P);
            break;
        }
        vector< pair<double, double> > newP;
        vector<int> cover;
        vector<bool> used(P.size(), false);
        cover.push_back(0);
        int current = 1;
        int step = 1;
        while (not used[0]) {
            if (not used[current]) {
                while (cover.size() > 1 && cmp(area(P[cover[cover.size() - 2]], P[cover.back()], P[current]), 0) <= 0) {
                    used[cover.back()] = false;
                    cover.pop_back();
                }
                cover.push_back(current);
                used[current] = true;
            }

            if (current == int(P.size()) - 1)
                step = -1;
            current += step;

        }
        cover.pop_back();

        if (cover.size() <= 1 and P.size() >= 2) {
            cover.clear();
            cover.push_back(0);
            cover.push_back(P.size() - 1);
        }

        used = vector<bool>(P.size(), false);

        polygons.push_back(vector< pair<double, double> >());
        for (auto &point : cover) {
            used[point] = true;
            polygons.back().push_back(P[point]);
        }

        for (int i = 0; i < int(P.size()); ++i)
            if (not used[i])
                newP.push_back(P[i]);
        swap(P, newP);
    }

    // got polygons yey, let's find out for each of them the symettry axes
    vector< vector<Line> > axes;
    for (auto &polygon : polygons) {
        axes.push_back(vector<Line>());

        if (polygon.size() == 1) {
            continue;
            // they have to pass through this point
        }

        if (polygon.size() == 2) {
            // there are always two axes
            axes.back().push_back(Line::fromPoints(polygon[0], polygon[1]));
            axes.back().push_back(Line::perpendicularBisector(polygon[0], polygon[1]));
            continue;
        }

        auto now = polygon;
        now.insert(now.end(), polygon.begin(), polygon.end());
        now.insert(now.end(), polygon.begin(), polygon.end()); // we triple, why? Don't know
                                                               // we do we do, so we don't have cases with modulo

        int N = polygon.size();
        // we choose the symetric for point 0 (or N)
        for (int i = N + 1; i < 2 * N; ++i) {
            Line current = Line::perpendicularBisector(now[N], now[i]);

            bool badLine = false;
            for (int k = 0; k < N; ++k)
                if (not current.symmetricalPoints(now[N - k], now[i + k])) {
                    badLine = true;
                    break;
                }

            if (not badLine)
                axes.back().push_back(current);
        }

        // we've got one extra line
        Line current = Line::perpendicularBisector(now[N - 1], now[N + 1]);
        bool badLine = false;
        for (int k = 0; k < N; ++k)
            if (not current.symmetricalPoints(now[N - k], now[N + k])) {
                badLine = true;
                break;
            }

        if (not badLine)
            axes.back().push_back(current);
    }

    map<Line, int> count;
    for (auto &row : axes)
        for (auto &line : row) 
            line.simplify();

    for (auto &row : axes) {
        sort(row.begin(), row.end());
        row.erase(unique(row.begin(), row.end()), row.end());
        for (auto &line : row)
            ++count[line];
    }

    if (polygons.back().size() == 1) { // the very very special case
        for (auto &pair : count)
            if (pair.first.calculate(polygons.back().back()) == 0)
                ++pair.second;
    }

    vector<Line> answer;
    for (auto &pair : count)
        if (pair.second == int(polygons.size()))
            answer.push_back(pair.first);

    cout << answer.size() << "\n";
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(8);
    for (auto &line : answer)
        cout << line.A << " " << line.B << " " << line.C << "\n";
}

