#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("subpermutari.in");
    ofstream cout("subpermutari.out");

    int N; cin >> N;
    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    vector< vector<int> > count(N, vector<int>(N, 1));
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            for (int k = 0; i + k < N && j + k < N; ++k) {
                int countI = 0, countJ = 0;
                for (int p = i; p < i + k; ++p)
                    if (V[p] < V[i + k])
                        ++countI;
                for (int p = j; p < j + k; ++p)
                    if (V[p] < V[j + k])
                        ++countJ;
                if (countI == countJ) {
                    count[i][i + k] += count[j][j + k];
                    count[j][j + k] = 0;
                } else
                    break;
            }

    int64_t answer = 0;
    for (int i = 0; i < N; ++i)
        for (int j = i; j < N; ++j)
            answer += count[i][j] * count[i][j];
    cout << answer << "\n";
}

