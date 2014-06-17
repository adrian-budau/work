#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N, p, q, r, s; cin >> N >> p >> q >> r >> s;
        vector<int> V(N);
        int64_t sum = 0;
        for (int i = 0; i < N; ++i) {
            V[i] = (1LL * i * p + q) % r + s;
            sum += V[i];
        }

        int64_t step;
        for (step = 1; step < sum; step <<= 1);
        int64_t answer;
        for (answer = 0; step; step >>= 1) {
            answer += step;

            int pos = 0;
            for (int j = 0; j < 3 && pos < N; ++j) {
                int64_t now = answer;
                while (pos < N && now >= V[pos])
                    now -= V[pos++];
            }
            if (pos == N)
                answer -= step;
        }

        ++answer;
        int64_t sizes[3] = {0, 0, 0};
        int pos = 0;
        for (int j = 0; j < 3 && pos < N; ++j) {
            int64_t now = answer;
            sizes[j] = 0;
            while (pos < N && now >= V[pos]) {
                now -= V[pos];
                sizes[j] += V[pos];
                ++pos;
            }
        }

        cout << "Case #" << test << ": ";
        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(12);
        cout << (long double)(sum - max(max(sizes[0], sizes[1]), sizes[2]))/(long double)(sum) << "\n";
    }
}
