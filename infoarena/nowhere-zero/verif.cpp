#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

double dis(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double area(double x1, double y1, double x2, double y2, double x3, double y3) {
    double rez =  x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

    return rez;
}

vector<int> used, height, up;
vector< vector<int> > E;
int total = 0;
int critical = 0;
void dfs(int node) {
    used[node] = 1;
    up[node] = height[node];
    ++total;

    bool can_parent = false;
    for (auto &next : E[node])
        if (used[next] == 0) {
            height[next] = height[node] + 1;
            dfs(next);

            if (up[next] == height[next]) {
                cerr << node << " < - > " << next << "\n";
                ++critical;
            } else
                up[node] = min(up[node], up[next]);
        } else {
            if (can_parent || height[next] != height[node] - 1) {
                up[node] = min(up[node], up[next]);
            } else
                can_parent = true;
        }
}

int main() {
    ifstream cin("nowherezero.in");

    int N, M; cin >> N >> M;

    vector<double> X(N), Y(N);
    for (int i = 0; i < N; ++i)
        cin >> X[i] >> Y[i];

    if (N < 20000) {
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j)
                if (dis(X[i], Y[i], X[j], Y[j]) < 1e-6)
                    return -1;
    }

    // so we don't have 2 close

    vector<int> A(M), B(M);
    E.resize(N);
    for (int i = 0; i < M; ++i) {
        cin >> A[i] >> B[i];
        --A[i]; --B[i];
        E[A[i]].push_back(B[i]);
        E[B[i]].push_back(A[i]);
    }

    height.resize(N, -1);
    up.resize(N, -1);
    used.resize(N, 0);

    height[0] = 0;
    dfs(0);

    if (critical > 0)
        return -3;

    if (total < N)
        return -4;

    cerr << " All fine\n";
    for (int i = 0; i < M; ++i)
        for (int j = i + 1; j < M; ++j) {
            if (A[i] == A[j] or A[i] == B[j] or B[i] == A[j] or B[i] == B[j])
                continue;

            int a1 = A[i], b1 = B[i], a2 = A[j], b2 = B[j];
            if (area(X[a1], Y[a1], X[b1], Y[b1], X[a2], Y[a2]) * area(X[a1], Y[a1], X[b1], Y[b1], X[b2], Y[b2]) < -1e-6)
                if (area(X[a2], Y[a2], X[b2], Y[b2], X[a1], Y[a1]) * area(X[a2], Y[a2], X[b2], Y[b2], X[b1], Y[b1]) < -1e-6) {
                    return -2;
                }
        }

    return 0;
}
