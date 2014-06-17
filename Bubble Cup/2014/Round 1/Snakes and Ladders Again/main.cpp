#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int getStable(vector<int> &unstable, int now) {
    if (not unstable[now])
        return now;

    return (unstable[now] = getStable(unstable, unstable[now]));
}

const double eps = 1e-4;
int cmp(const double &a, const double& b) {
    if (a + eps < b)
        return -1;
    if (b + eps < a)
        return 1;
    return 0;
}

int main() {
    int N, M, S, L;
    while (cin >> N >> M >> S >> L) {
        N *= M;
        ++N;
        vector<int> unstable(N, 0);
        for (int i = 0; i < S; ++i) {
            int x, y; cin >> x >> y;
            unstable[x] = y;
        }

        for (int i = 0; i < L; ++i) {
            int x, y; cin >> x >> y;
            unstable[x] = y;
        }

        for (int i = 1; i < N; ++i)
            getStable(unstable, i);

        vector< vector<int> > edges(N);
        vector<bool> used(N, false);
        used[0] = true;
        queue<int> Q;
        Q.push(0);
        while (!Q.empty()) {
            int i = Q.front();
            Q.pop();
            for (int j = 1; j <= 6; ++j)
                if (i + j < N) {
                    int next = getStable(unstable, i + j);
                    if (used[next])
                        continue;
                    Q.push(next);
                    used[next] = true;
                }
        }

        vector< vector<double> > matrix(N, vector<double>(N + 1, 0.0));
        for (int i = 0; i < N; ++i)
            if (used[i]) {
                matrix[i][i] = 1;
                matrix[i][N] = 1;
            }

        for (int i = 0; i + 1 < N; ++i)
            if (used[i])
                for (int j = 1; j <= 6; ++j) {
                    if (i + j < N) {
                        int next = getStable(unstable, i + j);
                        matrix[i][next] -= 1.0 / 6.0;
                    } else {
                        matrix[i][i] -= 1.0 / 6.0;
                    }
                }
        matrix[N - 1][N] = 0;

        int finished = 0;
        for (int i = 0; i < N; ++i) {
            int found = -1;
            for (int j = finished; j < N; ++j)
                if (cmp(matrix[j][i], 0)) {
                    found = j;
                    break;
                }

            if (found == -1)
                continue;
            swap(matrix[finished], matrix[found]);
            for (int j = 0; j < N; ++j)
                if (j != finished && cmp(matrix[j][i], 0))
                    for (int k = N; k >= i; --k)
                        matrix[j][k] -= matrix[finished][k] / matrix[finished][i] * matrix[j][i];
            ++finished;
        }

        double answer = matrix[0][N] / matrix[0][0];
        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(3);
        cout << answer << "\n";
    }
}
