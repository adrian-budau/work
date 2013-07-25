#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

const int kInfinite = 0x3f3f3f3f;

inline void minimize(int &a, int b) {
    if (b < a)
        a = b;
}

inline int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

inline int min(int a, int b) {
    if (a > b)
        return b;
    return a;
}

const int kMaxA = 31;
int past[kMaxA][kMaxA][kMaxA][kMaxA], present[kMaxA][kMaxA][kMaxA][kMaxA];

int main() {
    int A, N; cin >> A >> N;

    vector< pair<int, int> > V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i].first >> V[i].second;

    sort(V.begin(), V.end());

    vector<bool> L(N, true), R(N, true), U(N, true), D(N, true);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            if (V[i].first == V[j].first)
                if (V[i].second < V[j].second) {
                    D[j] = false;
                    U[i] = false;
                }
            if (V[i].second == V[j].second)
                if (V[i].first < V[j].first) {
                    L[j] = false;
                    R[i] = false;
                }
        }

    memset(present, 0x3f, sizeof(present));

    // present[i][j][k][l] the minimum cost so at the moment the highest DOWN is at i, the lowest UP is at j, the highest right is at k, and the lowest k is at l
    present[0][A][0][A] = 0;
    for (int i = 0; i < N; ++i) {
        memcpy(past, present, sizeof(past));
        memset(present, 0x3f, sizeof(present));

        for (int j = 0; j <= A; ++j)
            for (int k = 0; k <= A; ++k)
                for (int p = 0; p <= A; ++p)
                    for (int l = 0; l <= A; ++l) {
                        if (L[i] and j < V[i].second and k > V[i].second)
                            minimize(present[j][k][p][l], past[j][k][p][l] + V[i].first);
                        if (R[i])
                            minimize(present[j][k][max(p, V[i].second)][min(l, V[i].second)], past[j][k][p][l] + A - V[i].first);
                        if (U[i] and V[i].second > p)
                            minimize(present[j][min(V[i].second, k)][p][l], past[j][k][p][l] + A - V[i].second);
                        if (D[i] and V[i].second < l)
                            minimize(present[max(j, V[i].second)][k][p][l], past[j][k][p][l] + V[i].second);
                    }
    }
    int answer = kInfinite;
    for (int i = 0; i <= A; ++i)
        for (int j = 0; j <= A; ++j)
            for (int k = 0; k <= A; ++k)
                for (int p = 0; p <= A; ++p)
                    minimize(answer, present[i][j][k][p]);

    if (answer == kInfinite)
        cout << "No solution\n";
    else
        cout << "The total length of wire used is " << answer << "\n";
}
