#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

static int64_t mask[101];
int64_t back(const vector<int>& A, int64_t current_mask, int position, int N) {
    if (3 * position > N) {
        int64_t answer = 0;
        for (int i = position; i <= N; ++i) {
            if (i < 2 * position || i % 2 == 1) { // it's not a double
                int x = A[i];
                if (current_mask & mask[i])
                    x = 0;
                int y = 0;
                if (2 * i <= N && (current_mask & mask[2 * i]) == 0)
                    y = A[2 * i];
                answer += max(x + y, max(x, 0));
            }
        }
        return answer;
    }

    int64_t best_value = numeric_limits<int64_t>::min();
    int now = A[position];
    if ((current_mask & mask[position]) == 0) {
        best_value = max(best_value, back(A, current_mask | (1LL << (position)), position + 1, N));
    } else
        now = 0;
    return max(best_value, now + back(A, current_mask, position + 1, N));
}

int main() {
    int N; cin >> N;
    for (int i = 1; i <= N; ++i)
        for (int j = 1; j < i; ++j)
            if (i % j == 0)
                mask[i] |= (1LL << j);

    vector<int> V(N + 1);
    for (int i = 1; i <= N; ++i) {
        cin >> V[i];
    }

    cout << back(V, 0, 1, N) << "\n";
}
