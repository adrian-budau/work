#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

static const int kModulo = 10000;

inline void add(int &x, int y) {
    x += y;
    if (x >= kModulo)
        x -= kModulo;
}

static const int kMaxK = 160;
int dp1[16][kMaxK], next1[16][kMaxK];
int dp2[2][kMaxK][kMaxK], next2[2][kMaxK][kMaxK];
int bits[16];

int main() {
    ifstream cin("cuburi.in");
    ofstream cout("cuburi.out");

    int N, K; cin >> N >> K;

    bits[0] = 0;
    for (int i = 1; i < 16; ++i)
        bits[i] = bits[i / 2] + i % 2;

    dp1[0][0] = 1;
    for (int i = 1; i <= N; ++i) {
        memset(next1, 0, sizeof(next1));
        memset(next2, 0, sizeof(next2));
        int maxt = 4 * (i - 1);
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < K && j <= maxt; ++j)
                for (int k = 0; k < 16; ++k)
                    if (k == 5 || k == 10) {
                        if ((i & k) == k) {
                            if (j + 2 < K)
                                add(next1[k][j + 2], dp1[i][j]);
                        } else if ((i & k) == 0) {
                            if (1 < K)
                                add(next2[1 - k % 2][1][1], dp1[i][j]);
                        } else {
                            if (j + 1 >= K || 1 >= K)
                                continue;
                            switch (i & k) {
                                case 1:
                                    add(next2[0][j + 1][1], dp1[i][j]);
                                    break;
                                case 4:
                                    add(next2[0][1][j + 1], dp1[i][j]);
                                    break;
                                case 2:
                                    add(next2[1][j + 1][1], dp1[i][j]);
                                    break;
                                case 8:
                                    add(next2[1][1][j + 1], dp1[i][j]);
                                    break;
                            }
                        }
                    }else {
                        if (k & i) {
                            if (bits[k] + j < K)
                                add(next1[k][j + bits[k]], dp1[i][j]);
                        } else {
                            if (bits[k] < K)
                                add(next1[k][bits[k]], dp1[i][j]);
                        }
                    }

        for (int i = 0; i < 2; ++i) {
            int mask = 5 << i;
            for (int j = 0; j < K && j <= maxt; ++j)
                for (int k = 0; k < K && k <= maxt; ++k)
                    for (int l = 0; l < 16; ++l)
                        if (l == mask) {
                            if (j + 1 < K && k + 1 < K)
                                add(next2[i][j + 1][k + 1], dp2[i][j][k]);
                        } else if (mask == 15 - l) {
                            if (1 < K)
                                add(next2[1 - i][1][1], dp2[i][j][k]);
                        } else {
                            switch (mask & l) {
                                case 0:
                                    if (bits[l] < K)
                                        add(next1[l][bits[l]], dp2[i][j][k]);
                                    break;
                                case 1:
                                case 2:
                                    if (j + bits[l] < K)
                                        add(next1[l][j + bits[l]], dp2[i][j][k]);
                                    break;
                                case 4:
                                case 8:
                                    if (bits[l] + k < K)
                                        add(next1[l][k + bits[l]], dp2[i][j][k]);
                                    break;
                                default:
                                    if (bits[l] + j + k < K)
                                        add(next1[l][bits[l] + j + k], dp2[i][j][k]);
                            }
                        }
        }
        memcpy(dp1, next1, sizeof(dp1));
        memcpy(dp2, next2, sizeof(dp2));
    }

    int answer = 1;
    for (int i = 1; i <= N; ++i)
        answer = 16 * answer % kModulo;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < K; ++j)
            answer = (answer - dp1[i][j]) % kModulo;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < K; ++j)
            for (int k = 0; k < K; ++k)
                answer = (answer - dp2[i][j][k]) % kModulo;
    if (answer < 0)
        answer += kModulo;

    cout << answer << "\n";
}
