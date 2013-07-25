#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include <algorithm>

using namespace std;


int main() {
    int A, B, N, M;
    cin >> A >> B >> N >> M;

    if (A > B)
        swap(A, B);
    vector< vector<int> > V(N, vector<int>(M));
    vector<int> U;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            cin >> V[i][j];

    long long answer = 0;

    vector<int> depth(M + 1);

    vector<int> D;
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < M; ++k)
            depth[k] = V[i][k];
        depth[M] = 0;
        for (int j = i; j < N; ++j) {
            if (j - i + 1 > B)
                break;
            for (int k = 0; k < M; ++k)
                depth[k] = min(depth[k], V[j][k]);
            int maxt;
            if (j - i + 1 <= A)
                maxt = B;
            else
                maxt = A;

            for (int k = 0; k <= M; ++k) {
                while (not D.empty() and depth[D.back()] >= depth[k]) {
                    int W;
                    int H = depth[D.back()];
                    D.pop_back();
                    if (D.size())
                        W = k - D.back() - 1;
                    else
                        W = k;
                    W = min(W, maxt);

                    int area = W * (j - i + 1);
                    long long max_height = (1LL * H * N * M + N * M - area - 1) / (N * M - area) - 1;
                    answer = max(answer, max_height * area);
                }

                D.push_back(k);
            }

            while (D.size())
                D.pop_back();
        }
    }

    cout << answer << "\n";
}
