#include <iostream>
#include <vector>
#include <limits>

using namespace std;

int main() {
    int T; cin >> T;

    while (T--) {
        int N; cin >> N;

        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];

        vector<long long> best_left_high(N, numeric_limits<long long>::min()), best_right_high = best_left_high,
                          best_left_low(N, numeric_limits<long long>::max()), best_right_low = best_left_low;

        long long best_high = numeric_limits<long long>::min(), small_sum = 0, sum = 0,
                  best_low = numeric_limits<long long>::max(), big_sum = 0;
        for (int i = 0; i < N; ++i) {
            sum += V[i];
            best_high = max(best_high, sum - small_sum);
            best_low = min(best_low, sum - big_sum);

            best_left_high[i] = best_high;
            best_left_low[i] = best_low;

            small_sum = min(small_sum, sum);
            big_sum = max(big_sum, sum);
        }

        best_high = numeric_limits<long long>::min(), small_sum = 0, sum = 0,
        best_low = numeric_limits<long long>::max(), big_sum = 0;

        for (int i = N - 1; i >= 0; --i) {
            sum += V[i];
            best_high = max(best_high, sum - small_sum);
            best_low = min(best_low, sum - big_sum);

            best_right_high[i] = best_high;
            best_right_low[i] = best_low;

            small_sum = min(small_sum, sum);
            big_sum = max(big_sum, sum);
        }

        long long answer = numeric_limits<long long>::min();
        for (int i = 1; i < N; ++i) {
            answer = max(answer, best_left_high[i - 1] - best_right_low[i]);
            answer = max(answer, best_right_high[i] - best_left_low[i - 1]);
        }

        cout << answer << "\n";
    }
}
