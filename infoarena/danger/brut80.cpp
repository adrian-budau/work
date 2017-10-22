#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <cassert>

using namespace std;

const auto noop = [](int, int, int){};

template<class T>
bool can(int answer, const vector< vector<int> > &V, T cb) {
    int N = V.size();
    int M = V[0].size();

    vector< multiset<int> > S(N);
    for (int i = 0; i < N; ++i)
        for (auto &x : V[i])
            S[i].emplace(x);

    for (int i = 0; i < M; ++i) {
        int maxt = numeric_limits<int>::min(), where = -1;
        for (int j = 0; j < N; ++j) {
            int current_max = *S[j].rbegin();
            if (current_max > maxt) {
                maxt = current_max;
                where = j;
            }
        }

        for (int j = 0; j < N; ++j)
            if (j == where) {
                cb(i, j, maxt);
                S[j].erase(S[j].find(maxt));
            } else {
                auto it = S[j].upper_bound(answer - maxt);
                if (it == S[j].begin())
                    return false;
                --it;
                cb(i, j, *it);
                S[j].erase(it);
            }
    }
    return true;
}

int main() {
    ifstream cin("danger.in");
    ofstream cout("danger.out");

    int N, M; cin >> N >> M;
    assert(N * M <= 15000);
    vector< vector<int> > V(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j)
            cin >> V[i][j];
    }

    int answer = 0;
    for (int step = (1 << 30); step; step >>= 1)
        if (!can(answer + step, V, noop))
            answer += step;

    vector< vector<int> > mat(M, vector<int>(N));
    can(answer + 1, V, [&](int x, int y, int v) {
        mat[x][y] = v;
    });
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j)
            cout << mat[i][j] << " ";
        cout << "\n";
    }
}

