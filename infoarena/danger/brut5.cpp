#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("danger.in");
    ofstream cout("danger.out");

    int N, M; cin >> N >> M;
    vector< vector<int> > V(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j)
            cin >> V[i][j];
        sort(V[i].begin(), V[i].end());
    }

    for (int i = 0; i < M; ++i)
        cout << V[0][i] << " " << V[1][M - i - 1] << "\n";
}
