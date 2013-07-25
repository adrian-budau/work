#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

const double eps = 1e-6;

int cmp(double x, double y) {
    if (x + eps < y)
        return -1;
    if (y + eps < x)
        return 1;
    return 0;
}

double distance(double X1, double Y1, double X2, double Y2) {
    return sqrt((X1 - X2) * (X1 - X2) + (Y1 - Y2) * (Y1 - Y2));
}

class Line {
  public:
    Line(double _AX, double _AY, double _BX, double _BY):
            AX(_AX), AY(_AY), BX(_BX), BY(_BY) {}
    double AX, AY, BX, BY;

};

class Figure {
  public:

    bool intersect(const Line& that) const {
        double LAB = distance(that.AX, that.AY, that.BX, that.BY);
        double DX = (that.BX - that.AX) / LAB;
        double DY = (that.BY - that.AY) / LAB;

        double t = DX * (X - that.AX) + DY * (Y - that.AY);

        if (t < 0)
            t = 0;
        if (t > LAB)
            t = LAB;

        double EX = t * DX + that.AX;
        double EY = t * DY + that.AY;

        if (cmp(distance(EX, EY, X, Y), R) <= 0)
            return true;
        return false;
    }

    bool intersect(const Figure& that) const {
        if (cmp(X, that.X) == 0 and cmp(Y, that.Y) == 0)
            return true;

        if (type == CIRCLE and that.type == CIRCLE) {
            if (cmp(distance(X, Y, that.X, that.Y), R + that.R) <= 0)
                return true;
            return false;
        }

        if (type == CIRCLE and that.type == SQUARE) {
            double L = that.R;
            Line A(that.X - L, that.Y - L, that.X - L, that.Y + L);
            Line B(that.X - L, that.Y + L, that.X + L, that.Y + L);
            Line C(that.X + L, that.Y + L, that.X + L, that.Y - L);
            Line D(that.X + L, that.Y - L, that.X - L, that.Y - L);

            if (intersect(A) or intersect(B) or intersect(C) or intersect(D))
                return true;
            return false;
        }

        if (type == SQUARE and that.type == CIRCLE)
            return that.intersect(*this);

        // SQUARE SQUARE
        double LA = R;
        double LB = that.R;

        double XA1 = X - LA, YA1 = Y - LA, XA2 = X + LA, YA2 = Y + LA;
        double XB1 = that.X - LB, YB1 = that.Y - LB, XB2 = that.X + LB, YB2 = that.Y + LB;

        double X1 = max(XA1, XB1);
        double X2 = min(XA2, XB2);
        double Y1 = max(YA1, YB1);
        double Y2 = min(YA2, YB2);

        if (cmp(X1, X2) <= 0 and cmp(Y1, Y2) <= 0)
            return true;
        return false;
    }

  protected:
    Figure(int _type, double _X, double _Y, double _R):
            type(_type), X(_X), Y(_Y), R(_R) {}
    enum {
        SQUARE = 1,
        CIRCLE = 2
    };

    int type;
    double X, Y, R;
};

class Square : public Figure {
  public:
    Square(double X, double Y, double R):
            Figure(Figure::SQUARE, X, Y, R) {
    }
};

class Circle : public Figure {
  public:
    Circle(double X, double Y, double R):
            Figure(Figure::CIRCLE, X, Y, R) {
    }
};

void dfs(int node, vector< vector<int> > &edges, vector<int> &used) {
    if (used[node])
        return;
    used[node] = 1;
    for (vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it)
        dfs(*it, edges, used);
}

int main() {
    int M, B; cin >> M >> B;

    map< pair<int, int>, int> Squares, Circles;
    for (int i = 0; i < M; ++i) {
        int a1, b1, a2, b2;
        char x;
        cin >> a1 >> x >> b1 >> a2 >> x >> b2;

        int X = a1 * 100 + b1, Y = a2 * 100 + b2;

        Circles[make_pair(X, Y)]++;
    }

    for (int i = 0; i < B; ++i) {
        int a1, b1, a2, b2;
        char x;
        cin >> a1 >> x >> b1 >> a2 >> x >> b2;

        int X = a1 * 100 + b1, Y = a2 * 100 + b2;

        Squares[make_pair(X, Y)]++;
    }

    vector<Figure> V;
    for (map< pair<int, int>, int>::iterator it = Squares.begin(); it != Squares.end(); ++it)
        V.push_back(Square(it -> first.first, it -> first.second, it -> second * 2500));

    for (map< pair<int, int>, int>::iterator it = Circles.begin(); it != Circles.end(); ++it)
        V.push_back(Circle(it -> first.first, it -> first.second, it -> second * 2500));

    int N = V.size();
    V[0].intersect(V[5]);

    vector< vector<int> > edges(N + 4);
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (V[i].intersect(V[j])) {
                edges[i].push_back(j);
                edges[j].push_back(i);
            }

    Line A1(0, 0, 50000, 0), B1(50000, 0, 50000, 50000), C1(50000, 50000, 0, 50000), D1(0, 50000, 0, 0);
    for (int i = 0; i < N; ++i) {
        if (V[i].intersect(A1)) {
            edges[i].push_back(N);
            edges[N].push_back(i);
        }

        if (V[i].intersect(B1)) {
            edges[i].push_back(N + 1);
            edges[N + 1].push_back(i);
        }

        if (V[i].intersect(C1)) {
            edges[i].push_back(N + 2);
            edges[N + 2].push_back(i);
        }

        if (V[i].intersect(D1)) {
            edges[i].push_back(N + 3);
            edges[N + 3].push_back(i);
        }
    }

    edges[N].push_back(N + 1);
    edges[N + 1].push_back(N);
    edges[N + 2].push_back(N + 3);
    edges[N + 3].push_back(N + 2);

    vector<int> used(N + 4, 0);
    dfs(N, edges, used);

    if (used[N + 2])
        cout << "Mission failed.\n";
    else
        cout << "Mission accomplished.\n";
}
