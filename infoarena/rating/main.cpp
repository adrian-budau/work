#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("rating.in");
    ofstream cout("rating.out");

    int N, M; cin >> N >> M;
    assert(1 <= N && 1 <= M && 1LL * N * M <= 1000 * 1000);
    vector< vector<int> > V(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            cin >> V[i][j];
            assert(0 <= V[i][j] && V[i][j] <= 1000 * 1000 * 1000);
        }

    vector<int> P(N);
    for (int i = 0; i < N; ++i)
        P[i] = i;
    for (int i = 0; i < M; ++i) {
        sort(P.begin(), P.end(), [&](int x, int y) {
            return V[x][i] > V[y][i];
        });
        for (int j = 0; j < N; ++j)
            cout << P[j] + 1 << " ";
        cout << "\n";
    }
}
