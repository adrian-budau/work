#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int kModulo = 666013;

int main() {
    ifstream cin("templu.in");
    ofstream cout("templu.out");

    int N, M; cin >> N >> M;

    vector< vector<int> > V(N + 1, vector<int>(N + 1));
    V[1][1] = 1;
    for (int i = 2; i <= N; ++i) {
        V[i][1] = V[i][i] = (V[i - 1][1] + V[i - 2][1]) % kModulo;
        for (int j = 2; j < i; ++j)
            V[i][j] = (V[i - 1][j - 1] + V[i - 1][j]) % kModulo;
    }

    cout << V[N][M] << "\n";
}
