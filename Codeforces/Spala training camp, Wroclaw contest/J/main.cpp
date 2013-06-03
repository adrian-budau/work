#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

const int kMaxK = 21;
const int kInfinite = 500;

int DP[2][kMaxK][kMaxK][kMaxK][kMaxK];

void minimize(int &x, int y) {
    if (y < x)
        x = y;
}

int main() {
    int N, A, B; cin >> N >> A >> B;

    for (int i = 0; i <= A; ++i)
        for (int j = 0; j <= B; ++j)
            for (int k = 0; k <= A; ++k)
                for (int l = 0; l <= B; ++l)
                    DP[0][i][j][k][l] = kInfinite;

    DP[0][0][0][0][0] = 0;

    vector<int> a(N + 1), b(N + 1);
    for (int i = 1; i <= N; ++i)
        cin >> a[i];
    for (int i = 1; i <= N; ++i)
        cin >> b[i];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= A; ++j)
            for (int k = 0; k <= B; ++k)
                for (int l = 0; l <= A; ++l)
                    for (int p = 0; p <= B; ++p)
                        DP[(i + 1) & 1][j][k][l][p] = kInfinite;

        for (int j = 0; j <= A; ++j)
            for (int k = 0; k <= B; ++k)
                for (int l = 0; l <= A; ++l)
                    for (int p = 0; p <= B; ++p) {
                        if (DP[i & 1][j][k][l][p] == kInfinite)
                            continue;
                        DP[i & 1][A][B][l][p] = min(DP[i & 1][A][B][l][p], DP[i & 1][j][k][l][p] + 1);
                        DP[i & 1][j][k][A][B] = min(DP[i & 1][j][k][A][B], DP[i & 1][j][k][l][p] + 1);

                        if (a[i + 1] == 1 and b[i + 1] == 1) {
                            if (j > 0 and k > 0 and l > 0 and p > 0)
                                minimize(DP[(i + 1) & 1][j - 1][k - 1][l - 1][p - 1], DP[i & 1][j][k][l][p]);
                        } else if (a[i + 1] + b[i + 1] == 1) {
                            if (l > 0 and p > 0)
                                minimize(DP[(i + 1) & 1][j][max(k - 1, 0)][l - 1][p - 1], DP[i & 1][j][k][l][p]);
                            if (j > 0 and k > 0)
                                minimize(DP[(i + 1) & 1][j - 1][k - 1][l][max(p - 1, 0)], DP[i & 1][j][k][l][p]);
                        } else {
                            minimize(DP[(i + 1) & 1][j][max(k - 1, 0)][l][max(p - 1, 0)], DP[i & 1][j][k][l][p]);
                        }
                    }
    }

    int answer = kInfinite;
    for (int i = 0; i <= A; ++i)
        for (int j = 0; j <= B; ++j)
            for (int k = 0; k <= A; ++k)
                for (int l = 0; l <= B; ++l)
                    answer = min(answer, DP[N & 1][i][j][k][l]);
    cout << answer << "\n";
}
