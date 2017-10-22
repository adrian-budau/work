#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("hashtag.in");
    ofstream cout("hashtag.out");

    int N, M; cin >> N >> M;
    vector<string> S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i];

    vector< vector<int> > dots(N + 1, vector<int>(M + 1, 0)), hash = dots;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (S[i][j] == '.')
                dots[i + 1][j + 1] = 1;
            else
                hash[i + 1][j + 1] = 1;
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= M; ++j) {
            dots[i][j] += dots[i - 1][j];
            hash[i][j] += hash[i - 1][j];
            dots[i][j] += dots[i][j - 1];
            hash[i][j] += hash[i][j - 1];
            dots[i][j] -= dots[i - 1][j - 1];
            hash[i][j] -= hash[i - 1][j - 1];
        }

    auto Hash = [&](int x1, int y1, int x2, int y2) {
        return hash[x2][y2] - hash[x1 - 1][y2] - hash[x2][y1 - 1] + hash[x1 - 1][y1 - 1];
    };

    auto Dots = [&](int x1, int y1, int x2, int y2) {
        return dots[x2][y2] - dots[x1 - 1][y2] - dots[x2][y1 - 1] + dots[x1 - 1][y1 - 1];
    };

    int answer = N * M;
    for (int i1 = 2; i1 < N; ++i1)
        for (int j1 = i1; j1 < N; ++j1)
            for (int i2 = j1 + 2; i2 < N; ++i2)
                for (int j2 = i2; j2 < N; ++j2)
                    for (int k1 = 2; k1 < M; ++k1)
                        for (int l1 = k1; l1 < M; ++l1)
                            for (int k2 = l1 + 2; k2 < M; ++k2)
                                for (int l2 = k2; l2  < M; ++l2) {
                                    int aux = Hash(1, 1, i1 - 1, k1 - 1) + Hash(1, l1 + 1, i1 - 1, k2 - 1) + Hash(1, l2 + 1, i1 - 1, M) +
                                              Hash(j1 + 1, 1, i2 - 1, k1 - 1) + Hash(j1 + 1, l1 + 1, i2 - 1, k2 - 1) + Hash(j1 + 1, l2 + 1, i2 - 1, M) +
                                              Hash(j2 + 1, 1, N, k1 - 1) + Hash(j2 + 1, l1 + 1, N, k2 - 1) + Hash(j2 + 1, l2 + 1, N, M);
                                    aux += Dots(1, k1, N, l1) + Dots(1, k2, N, l2);
                                    aux += Dots(i1, 1, j1, k1 - 1) + Dots(i1, l1 + 1, j1, k2 - 1) + Dots(i1, l2 + 1, j1, M);
                                    aux += Dots(i2, 1, j2, k1 - 1) + Dots(i2, l1 + 1, j2, k2 - 1) + Dots(i2, l2 + 1, j2, M);
                                    answer = min(answer, aux);
                                }

    cout << answer << "\n";
}
