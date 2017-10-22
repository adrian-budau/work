#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    vector< vector<int> > A(N + 1, vector<int>(N, 0));

    for (int i = 1; i <= N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> A[i][j];

    auto downLeft = A;
    auto downRight = A;
    auto left = A;
    auto right = A;
    int answer = A[0][0];
    for (int i = 1; i <= N; ++i) {
        for (int j = 0; j < N; ++j) {
            downLeft[i][j] = max(downLeft[i][j], max(downLeft[i - 1][j], left[i - 1][j]) + A[i][j]);
            downRight[i][j] = max(downRight[i][j], max(downRight[i - 1][j], right[i - 1][j]) + A[i][j]);
            if (j > 0)
                left[i][j] = max(left[i][j], max(downRight[i][j - 1], left[i][j - 1]) + A[i][j]);
        }

        for (int j = N - 1; j >= 0; --j) {
            if (j < N - 1)
                right[i][j] = max(right[i][j], max(downLeft[i][j + 1], right[i][j + 1]) + A[i][j]);
            answer = max({answer, left[i][j], right[i][j], downLeft[i][j], downRight[i][j]});
        }
    }

    cout << answer << "\n";
}
