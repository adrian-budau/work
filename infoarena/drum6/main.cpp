#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("drum6.in");
    ofstream cout("drum6.out");

    int N, M; cin >> N >> M;

    vector<string> S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i];

    vector< vector<bool> > good(N, vector<bool>(M, false));
    good[0][0] = 1;
    for (int i = 0; i < N + M - 1; ++i) {
        char best = 'z' + 1;
        for (int x = i, y = 0; x >= 0; --x, ++y)
            if (x < N && y < M)
                if (good[x][y])
                    best = min(best, S[x][y]);
        cout << best;

        for (int x = i, y = 0; x >= 0; --x, ++y)
            if (x < N && y < M)
                if (good[x][y] && S[x][y] == best) {
                    if (x + 1 < N)
                        good[x + 1][y] = true;
                    if (y + 1 < M)
                        good[x][y + 1] = true;
                }
    }
    cout << "\n";
}
