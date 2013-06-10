#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <complex>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
#include <cassert>

#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

template<class T>
T from_string(const string& S) {
    istringstream in(S);
    T value; in >> value;
    return value;
}

int points;

vector<double> CX, CY;
vector< pair<int, int> > edges;

class Point {
  public:
    Point(const double &_x = 0, const double &_y = 0, const int &_index = 0):
            x(_x),
            y(_y),
            index(_index) {
        if (index == int(CX.size())) {
            CX.push_back(x);
            CY.push_back(y);
        }
    }

    double x, y;
    int index;
};

double dis(const Point& a, const Point &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double area(const Point& a, const Point &b, const Point &c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

vector<Point> Polygon(const int &size, const double &CMAX) {
    // so we wont risk we'll actualy generate in CMAX / size
    double limit = CMAX / size;

    vector<Point> now;
    for (int i = 0; i < size / 2; ++i) {
        double X, Y;
        do {
            X = randomDouble(1e-6, limit);
            Y = randomDouble(1e-6, limit);

            bool ok = true;
            for (auto &other : now) {
                if (dis(other, Point{X, Y, 0}) < 1) {
                    ok = false;
                    break;
                }

                if (fabs(Y / X - other.y / other.x) < 1) {
                    ok = false;
                    break;
                }
            }

            if (ok) {
                now.push_back({X, Y, 0});
                break;
            }
        } while (true);
    }

    now.resize(size);
    for (int i = 0; i < size / 2; ++i)
        now[i + size / 2] = {-now[i].x, -now[i].y, 0};

    sort(now.begin(), now.end(), [](const Point& a, const Point& b) {
        return atan2(a.y, a.x) < atan2(b.y, b.x);
    });

    vector<Point> result;
    double Xnow = 0, Ynow = 0;
    for (int i = 0; i < size; ++i) {
        result.push_back({Xnow, Ynow, ++points});
        Xnow -= now[i].x;
        Ynow -= now[i].y;
    }

    return result;
}

int stack_size = 0;
void populate(vector<Point> polygon, int need) {
    assert(polygon.size() > 2);
    if (need == 0) {
        --stack_size;
        return;
    }

    int taken = randomInt<int>(3, polygon.size());
    auto vertexes = randomSubsequence(polygon, taken);

    double X = 0, Y = 0;
    for (auto &point : vertexes) {
        X += point.x;
        Y += point.y;
    }

    X /= taken;
    Y /= taken;

    Point center{X, Y, ++points};

    taken = randomInt<int>(2, polygon.size());
    auto which = randomSample<int>(taken, 0, polygon.size() - 1); // to whom do we connect
    for (int i = 0; i < taken; ++i)
        edges.push_back({polygon[which[i]].index, center.index});

    --need;
    if (need == 0) {
        --stack_size;
        return;
    }

    vector< vector<Point> > newPolygons;
    for (int i = 0; i < taken - 1; ++i) {
        vector<Point> newPolygon(polygon.begin() + which[i], polygon.begin() + which[i + 1] + 1);
        if (fabs(area(polygon[which[i]], polygon[which[i + 1]], center)) > 1e-6)
            newPolygon.push_back(center);
        if (newPolygon.size() > 2 and area(polygon[which[i]], polygon[which[i + 1]], center) > -1e-6)
            newPolygons.push_back(newPolygon);
    }

    vector<Point> newPolygon(polygon.begin() + which.back(), polygon.end());
    newPolygon.insert(newPolygon.end(), polygon.begin(), polygon.begin() + which[0] + 1);
    if (fabs(area(polygon[which.front()], polygon[which.back()], center)) > 1e-6)
        newPolygon.push_back(center);
    if (newPolygon.size() > 2 and area(polygon[which.back()], polygon[which.front()], center) > -1e-6)
        newPolygons.push_back(newPolygon);


    // half of what i need is by default given so we can drop down fast
    int toeach = (need / 2) / newPolygons.size();
    int extra = (need / 2) % newPolygons.size();

    if (stack_size > 5) {  
        toeach = need / newPolygons.size();
        extra = need % newPolygons.size();
        need = 0;
    } else
        need = need - (need / 2);
    auto partition = randomPartition<int>(need + newPolygons.size(), newPolygons.size());
    for (auto &x : partition) {
        --x;
        x += toeach;
        if (extra) {
            ++x;
            --extra;
        }
    }

    for (int i = 0; i < int(newPolygons.size()); ++i)
        populate(newPolygons[i], partition[i]);
    --stack_size;
}

int main(int argv, char *argc[]) {
    int BEGIN = from_string<int>(argc[1]);
    // we're sorry but we need BEGIN even
    BEGIN += BEGIN % 2;
    int END = from_string<int>(argc[2]);
    // so we need END >= BEGIN
    if (END < BEGIN)
        END = BEGIN;
    double CMAX = from_string<double>(argc[3]);

    if (argv > 4)
        reSeed(from_string<unsigned>(argc[4]));

    auto start = Polygon(BEGIN, CMAX);

    for (int i = 0; i < BEGIN - 1; ++i)
        edges.push_back({start[i].index, start[i + 1].index});
    edges.push_back({start.front().index, start.back().index});

    populate(start, END - BEGIN);

    // end shuffle
    vector<int> P(points + 1);
    for (int i = 1; i <= points; ++i)
        P[i] = i;
    randomShuffle(P.begin() + 1, P.end());

    vector<int> WHO(points + 1);
    for (int i = 1; i <= points; ++i)
        WHO[P[i]] = i;

    cout << points << " " << edges.size() << "\n";

    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(6);
    for (int i = 1; i <= points; ++i)
        cout << CX[P[i]] << " " << CY[P[i]] << "\n";

    edges = shuffle(edges);
    for (int i = 0; i < int(edges.size()); ++i) {
        int x = WHO[edges[i].first];
        int y = WHO[edges[i].second];
        if (randomElement({true, false}))
            swap(x, y);
        cout << x << " " << y << "\n";
    }
}

