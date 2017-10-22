#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("fortmoo.in");
    ofstream cout("fortmoo.out");
    int N, M; cin >> N >> M;

    vector<string> S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i];

    vector< vector<int> > up(N, vector<int>(M, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (S[i][j] == '.')
                up[i][j] = 1 + (i > 0 ? up[i - 1][j] : 0);
            else
                up[i][j] = 0;

    int answer = 0;
    for (int i = 0; i < N; ++i)
        for (int j = i; j < N; ++j) {
            int good = -1;
            for (int k = 0; k < M; ++k) {
                if (up[j][k] > j - i) {
                    if (good == -1) {
                        answer = max(answer, j - i + 1);
                        good = k;
                    } else
                        answer = max(answer, (j - i + 1) * (k - good + 1));
                }
                if (S[i][k] == 'X' || S[j][k] == 'X')
                    good = -1;
            }
        }
    cout << answer << "\n";
}
