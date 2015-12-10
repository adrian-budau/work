#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>

using namespace std;

static const double kEps = 1e-6;

int cmp(double a, double b) {
    if (a + kEps < b)
        return -1;
    if (b + kEps < a)
        return 1;
    return 0;
}

struct comp {
    bool operator()(pair<double, double> a, pair<double, double> b) {
        if (cmp(a.first, b.first) == -1)
            return true;
        if (cmp(a.first, b.first) == 0)
            return cmp(a.second, b.second) == -1;
        return false;
    }
};

int dfs(int node, const vector< vector<int> > &edge, vector<bool> &used, const vector< set< pair<double, double>, comp> > &points, set <pair<double, double>, comp> &current) {
    used[node] = true;
    int temp = points[node].size();
    for (auto &p : points[node])
        current.emplace(p);

    for (auto &x : edge[node])
        if (!used[x])
            temp += dfs(x, edge, used, points, current);
    return temp;
}

int main() {
    ifstream cin("cerc.in");
    ofstream cout("cerc.out");

    int N; cin >> N;
    vector<int> X(N), Y(N), R(N);
    for (int i = 0; i < N; ++i)
        cin >> X[i] >> Y[i] >> R[i];

    vector< set< pair<double, double>, comp> > points(N);
    vector<bool> copied(N, false);
    vector< vector<int> > edge(N);
    for (int i = 0; i < N; ++i) {
        bool copy = false;
        for (int j = 0; j < i; ++j)
            if (X[i] == X[j] && Y[i] == Y[j] && R[i] == R[j])
                copy = true;
        if (copy) {
            copied[i] = true;
            continue;
        }
        for (int j = 0; j < i; ++j)
            if (!copied[j]) {
                int distance = (X[i] - X[j]) * (X[i] - X[j]) + (Y[i] - Y[j]) * (Y[i] - Y[j]);
                if (distance == 0)
                    continue;
                double A = 2 * X[i] - 2 * X[j];
                double B = 2 * Y[i] - 2 * Y[j];
                double C = X[j] * X[j] - X[i] * X[i] + Y[j] * Y[j] - Y[i] * Y[i] + R[i] * R[i] - R[j] * R[j];

                double div = max(abs(A), abs(B));

                A /= div;
                B /= div;
                C /= div;

                if (cmp(A, 0)) {
                    double Aprim = B * B + A * A;
                    double Bprim = 2 * B * C + 2 * X[i] * A * B - 2 * Y[i] * A * A;
                    double Cprim = C * C + 2 * A * C * X[i] + X[i] * X[i] * A * A + Y[i] * Y[i] * A * A - R[i] * R[i] * A * A;

                    double delta = Bprim * Bprim - 4 * Aprim * Cprim;
                    if (cmp(delta, 0) == -1)
                        continue;
                    if (cmp(delta, 0) == 0)
                        delta = 0;
                    else
                        delta = sqrt(delta);

                    double Y1 = (-Bprim - delta) / 2 / Aprim;
                    double Y2 = (-Bprim + delta) / 2 / Aprim;
                    double X1 = (-C - B * Y1) / A;
                    double X2 = (-C - B * Y2) / A;
                    points[i].emplace(X1, Y1);
                    points[j].emplace(X1, Y1);
                    points[i].emplace(X2, Y2);
                    points[j].emplace(X2, Y2);
                    edge[i].emplace_back(j);
                    edge[j].emplace_back(i);
                } else {
                    double Aprim = A * A + B * B;
                    double Bprim = 2 * A * C + 2 * Y[i] * B * A - 2 * X[i] * B * B;
                    double Cprim = C * C + 2 * B * C * Y[i] + Y[i] * Y[i] * B * B + X[i] * X[i] * B * B - R[i] * R[i] * B * B;

                    double delta = Bprim * Bprim - 4 * Aprim * Cprim;
                    if (cmp(delta, 0) == -1)
                        continue;
                    if (cmp(delta, 0) == 0)
                        delta = 0;
                    else
                        delta = sqrt(delta);

                    double X1 = (-Bprim - delta) / 2 / Aprim;
                    double X2 = (-Bprim + delta) / 2 / Aprim;
                    double Y1 = (-C - A * X1) / B;
                    double Y2 = (-C - A * X2) / B;
                    points[i].emplace(X1, Y1);
                    points[j].emplace(X1, Y1);
                    points[i].emplace(X2, Y2);
                    points[j].emplace(X2, Y2);
                    edge[i].emplace_back(j);
                    edge[j].emplace_back(i);
                }
            }
    }

    vector<bool> used(N, false);
    int answer = 0;
    for (int i = 0; i < N; ++i)
        if (!used[i] && !copied[i]) {
            set< pair<double, double>, comp> current;
            answer += dfs(i, edge, used, points, current);
            answer -= current.size();
            ++answer;
        }
    cout << answer << "\n";
}
