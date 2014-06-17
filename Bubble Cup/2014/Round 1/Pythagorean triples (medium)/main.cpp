#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int main() {
    int T; cin >> T;

    while (T--) {
        int N, M; cin >> N >> M;

        long long answer = 0;
        for (int m = 2; m * m <= M; ++m)
            for (int n = m % 2 + 1; n < m; n += 2) {
                if (gcd(m, n) != 1)
                    continue;

                int a = min(m * m - n * n, 2 * m * n);
                int c = m * m + n * n;
                if (c > M)
                    break;

                int least = (N - 1) / a + 1;
                int most = M / c;
                if (least <= most)
                    answer += most - least + 1;
            }
        cout << answer << "\n";
    }
}
