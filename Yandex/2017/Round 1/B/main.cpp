#include <iostream>
#include <fstream>

using namespace std;

int main() {
    int64_t N, A; cin >> N >> A;
    int best = 0;
    for (int64_t ans = A; ans <= A + 2000 && ans + A <= N; ++ans) {
        int64_t right = N - ans;
        int now = 0;
        if (ans % 500)
            now += 500 - ans % 500;
        if (right % 500)
            now += 500 - right % 500;
        best = max(best, now);
    }
    cout << best << "\n";
}
