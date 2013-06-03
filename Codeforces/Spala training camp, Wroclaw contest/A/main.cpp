#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int N, K; cin >> N >> K;

    vector<int> S(N);
    vector< bitset<10> > B(N);
    int many = 1;
    for (int i = 0; i < K; ++i)
        many *= 3;
    vector< vector<int> > A(many);

   for (int i = 0; i < N; ++i) {
        cin >> S[i];
        int y; cin >> y;
        for (int j = 0; j < y; ++j) {
            int x; cin >> x;
            --x;
            B[i][x] = 1;
        }
    }

    vector<int> P(N);
    for (int i = 0; i < N; ++i)
        P[i] = i;
    sort(P.begin(), P.end(), [&](int x, int y) {
        return S[x] > S[y];
    });

    for (int i = 0; i < N; ++i) {
        int who = P[i];
        for (int j = 0; j < (1 << K); ++j) {
            int stare = 0;
            for (int k = 0; k < K; ++k) {
                if ((1 << k) & j)
                    stare = stare * 3 + 2;
                else
                    stare = stare * 3 + B[who][k];
            }
            A[stare].push_back(S[who]);
        }
    }

    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        int a, y; cin >> a >> y;
        --a;

        vector<int> state(K, 2);
        for (int j = 0; j < y; ++j) {
            int x; cin >> x;
            --x;
            state[x] = 0;
        }

        for (int j = 0; j < K; ++j)
            if (state[j] == 0)
                state[j] = B[a][j];


        int stare = 0;
        for (int j = 0; j < K; ++j)
            stare = stare * 3 + state[j];
        cout << lower_bound(A[stare].begin(), A[stare].end(), S[a], greater<int>()) - A[stare].begin() + 1 << "\n";
    }
}
