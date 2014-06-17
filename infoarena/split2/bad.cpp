#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <array>
 
using namespace std;
 
int main() {
    ifstream cin("split2.in");
    ofstream cout("split2.out");
 
    int T; cin >> T;
    assert(1 <= T and T <= 50);
 
    while (T--) {
        int N, M; cin >> N >> M;
        assert(N % 2 == 0 and N >= 1 and N <= 1000);
        assert(M * 2 <= N and M >= 1 and M <= 500);
 
        vector<int> V(N);
        for (int i = 0; i < N; ++i) {
            cin >> V[i];
            assert(1 <= V[i] and V[i] <= 10000);
        }
 
        int sum = accumulate(V.begin(), V.end(), 0);
        int step;
 
        for (step = 1; step < sum; step <<= 1);
        int answer;
        for (answer = 0; step; step >>= 1) {
            answer += step;
 
            vector<array<int, 2> > dp(N, {N, N});
            for (int i = 1; i < N; i += 2) {
                int left = 0, right = 0;
                for (int j = 1; 2 * j < i + 2; ++j) {
                    right += V[i - j + 1];
                    left += V[i - 2 * j + 1] + V[i - 2 * j + 2];
                    left -= V[i - j + 1];
                    if (right > answer)
                        break;
                    if (left > answer)
                        continue;
                    for (int k = 0; k < 2; ++k)
                        dp[i][k] = min(dp[i][k], i - 2 * j >= 0 ? dp[i - 2 * j][1 - k] + 1 : 1);
                }
            }
 
            if (dp[N - 1][M % 2] <= M)
                answer -= step;
        }
 
        cout << answer + 1 << "\n";
    }
}
