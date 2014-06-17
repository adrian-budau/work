#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

inline int max(int a, int b) {
    return (a < b) ? b : a;
}

inline int min(int a, int b) {
    return (a < b) ? a : b;
}

int best(const vector< vector<int> > &A, bool extra = false) {
    int N = A.size(), M = A[0].size();

    vector<int> down(N, 0); // down[i] the best matrix sum from row i downwards
    for (int i = N - 1; i >= 0; --i) {
        vector<int> sum(M, 0);
        for (int j = i; j < N; ++j) {
            int best = 0;
            if (i + 1 < N)
                best = max(best, down[i + 1]);

            int sum_now = 0;
            for (int k = 0; k < M; ++k) {
                sum[k] += A[j][k];
                sum_now = max(sum_now + sum[k], 0);
                best = max(best, sum_now);
            }

            down[i] = max(down[i], best);
        }
    }

    // now i need up[i] th sum of the best 2 matries from row i upwards
    vector<int> up(N, 0);
    vector<int> partial_up(N, 0);

    int answer = 0;
    vector<int> left(M, 0), right(M, 0); // the best matrix to the left and to the right;
    for (int i = 0; i < N; ++i) {
        if (i > 0) {
            up[i] = up[i - 1];
            partial_up[i] = partial_up[i - 1];
        }

        vector<int> sum(M, 0);
        for (int j = i; j >= 0; --j) {

            int sum_now = 0;
            for (int k = 0; k < M; ++k) {
                sum[k] += A[j][k];
                sum_now = max(sum_now + sum[k], 0);

                left[k] = max(left[k], sum_now);
                if (extra) {
                    answer = max(answer, sum_now + (j > 0 ? partial_up[j - 1] : 0) + (i + 1 < N ? down[i + 1] : 0));
                    partial_up[i] = max(partial_up[i], sum_now);
                }
            }

            sum_now = 0;
            for (int k = M - 1; k >= 0; --k) {
                sum_now = max(sum_now + sum[k], 0);
                right[k] = max(right[k], sum_now);
            }
        }

        int best = 0;
        for (int j = 1; j < M; ++j) {
            best = max(best, left[j - 1]);
            up[i] = max(up[i], best + right[j]);
        }
    }

    for (int i = 1; i < N; ++i)
        answer = max(answer, up[i - 1] + down[i]);
    return answer;
}

int main() {
    ifstream cin("treid.in");
    ofstream cout("treid.out");

    int N, M; cin >> N >> M;

    vector< vector<int> > A(N, vector<int>(M)), B(M, vector<int>(N)), C(A), D(B);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            cin >> A[i][j];
            B[j][i] = C[N - i - 1][j] = D[M - j - 1][i] = A[i][j];
        }

    cout << max({best(A, true), best(B, true), best(C), best(D)}) << "\n";
}
