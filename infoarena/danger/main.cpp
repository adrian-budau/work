#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

const auto noop = [](int, int, int){};

template<class T>
bool can(int answer, const vector< vector<int> > &V, vector< vector<int> > &backup, T cb) {
    int N = V.size();
    int M = V[0].size();
    vector<int> left(N, 0), right(N, M - 1);

    for (int i = 0; i < N; ++i)
        backup[i].clear();

    for (int i = 0; i < M; ++i) {
        int maxt = numeric_limits<int>::min(), where = -1;
        for (int j = 0; j < N; ++j) {
            int current_max;
            if (right[j] >= left[j])
                current_max = V[j][right[j]];
            else
                current_max = backup[j].back();
            if (current_max > maxt) {
                maxt = current_max;
                where = j;
            }
        }
        for (int j = 0; j < N; ++j)
            if (j == where) {
                cb(i, j, maxt);
                if (right[j] >= left[j])
                    --right[j];
                else
                    backup[j].pop_back();
            } else {
                while (left[j] <= right[j] && V[j][left[j]] + maxt <= answer) {
                    backup[j].push_back(V[j][left[j]]);
                    ++left[j];
                }
                if (backup[j].empty())
                    return false;
                cb(i, j, backup[j].back());
                backup[j].pop_back();
            }
    }
    return true;
}

int main() {
    ifstream cin("danger.in");
    ofstream cout("danger.out");

    int N, M; assert(cin >> N >> M);
    assert(2 <= N && N <= 100 * 1000);
    assert(2 <= M && M <= 100 * 1000);
    assert(1LL * N * M <= 100 * 1000);

    vector< vector<int> > V(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            assert(cin >> V[i][j]);
            assert(1 <= V[i][j] && V[i][j] <= 1000 * 1000 * 1000);
        }
        sort(V[i].begin(), V[i].end());
    }

    vector< vector<int> > backup(N);
    for (int i = 0; i < N; ++i)
        backup[i].reserve(M);

    int answer = 0;
    for (int step = (1 << 30); step; step >>= 1)
        if (!can(answer + step, V, backup, noop))
            answer += step;

    vector< vector<int> > mat(M, vector<int>(N));
    can(answer + 1, V, backup, [&](int x, int y, int v) {
        mat[x][y] = v;
    });
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j)
            cout << mat[i][j] << " ";
        cout << "\n";
    }
}

