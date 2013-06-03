#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <iostream>

using namespace std;

double distance(int x1, int y1, int z1, int x2, int y2, int z2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

struct functor {
    functor(vector<int>& data):
        data_(data) {
    }

    bool operator()(const int &x, const int &y) const {
        return data_[x] < data_[y];
    }

  private:
    const vector<int>& data_;
};

void dfs(int node, vector< vector<double> > &dis, vector<int> &type) {
    for (int i = 0; i < int(dis.size()); ++i)
        if (type[i] == -1 and dis[node][i] < 1e-6) {
            type[i] = type[node];
            dfs(i, dis, type);
        }
}

int main() {
    int N, M;
    int test = 0;
    long long operations = 0;
    while (scanf("%d %d", &N, &M) == 2) {
        operations += N * N * N;
        assert(operations <= 1000000000);
        ++test;

        vector< vector<double> > dis(N, vector<double>(N, 1e50));

        vector<int> X(N), Y(N), Z(N), K(N);
        for (int i = 0; i < N; ++i) {
            scanf("%d %d %d %d", &X[i], &Y[i], &Z[i], &K[i]);
            dis[i][i] = 0;
        }

        for (int i = 0; i < M; ++i) {
            int x, y; scanf("%d %d", &x, &y);
            --x; --y;
            dis[x][y] = dis[y][x] = 0;
        }

        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (dis[i][j] > 1e40 and K[i] > 0 and K[j] > 0)
                    dis[i][j] = distance(X[i], Y[i], Z[i], X[j], Y[j], Z[j]);

        vector<int> P(N);
        for (int i = 0; i < N; ++i)
            P[i] = i;
        sort(P.begin(), P.end(), functor(Z));

        int where = 0;
        for (int i = 0; i < N; ++i)
            if (P[i] == 0 or P[i] == N - 1)
                where = i;

        double answer = 1e50;
        vector< vector<double> > newDis(N, vector<double>(N, 1e50));
        for (int i = 0; i < where; ++i)
            for (int j = 0; j < where; ++j)
                newDis[P[i]][P[j]] = dis[P[i]][P[j]];
        for (int i = where; i < N; ++i) {
            int height = Z[P[i]];
            for (int k = 0; k < N; ++k)
                if (Z[k] <= height)
                    newDis[P[i]][k] = newDis[k][P[i]] = dis[P[i]][k];

            vector<int> type(N, -1);
            int components = 0;
            type[0] = 0;
            dfs(0, newDis, type);
            ++components;
            if (type[N - 1] == -1) {
                type[N - 1] = 1;
                dfs(N - 1, newDis, type);
                ++components;
            }

            for (int j = 1; j < N - 1; ++j)
                if (Z[j] <= height and type[j] == -1) {
                    type[j] = components;
                    dfs(j, newDis, type);
                    ++components;
                }

            vector<double> total(components, 0);
            for (int j = 0; j < N; ++j)
                if (Z[j] <= height)
                    total[type[j]] += double(K[j]) / 2;

            vector< vector<double > > compDis(components, vector<double>(components, 1e50));
            for (int j = 0; j < N; ++j)
                for (int k = 0; k < N; ++k)
                    if (Z[j] <= height and Z[k] <= height and type[j] != type[k])
                        compDis[type[j]][type[k]] = min(compDis[type[j]][type[k]], newDis[j][k] - 1 + total[type[k]]);

            for (int j = 0; j < components; ++j) {
                double enough = 1;
                if (j == type[0])
                    enough -= 0.5;
                if (j == type[N - 1])
                    enough -= 0.5;
                if (total[j] < enough - 1e-6)
                    for (int k = 0; k < components; ++k)
                        if (j != k)
                            compDis[j][k] = compDis[k][j] = 1e50;
            }

            vector<double> dis(components, 1e50);
            vector<int> used(components, 0);
            dis[0] = total[0];

            for (int j = 0; j < components; ++j) {
                if (used[type[N - 1]])
                    break;
                double lowest = 1e50; int where = -1;
                for (int k = 0; k < components; ++k)
                    if (not used[k])
                        if (dis[k] < lowest) {
                            lowest = dis[k];
                            where = k;
                        }

                if (lowest > answer)
                    break;
                if (lowest > 1e40)
                    break;
                used[where] = 1;
                for (int k = 0; k < components; ++k)
                    if (not used[k])
                        dis[k] = min(dis[k], dis[where] + compDis[where][k]);
            }

            answer = min(answer, dis[type[N - 1]]);
        }

        printf("Case %d: ", test);
        if (answer > 1e40)
            printf("impossible\n");
        else
            printf("%.4lf\n", answer);
    }
}
