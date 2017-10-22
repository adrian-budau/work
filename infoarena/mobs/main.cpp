#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

int64_t subsequence(const vector<int64_t>& sequence, int from, int to) {
    if (from > to)
        return 0;
    if (to < 0)
        return 0;
    int64_t v = sequence[to];
    if (from > 0)
        v -= sequence[from - 1];
    return v;
}

int main() {
    ifstream cin("mobs.in");
    ofstream cout("mobs.out");

    int T; cin >> T;
    while (T--) {
        int N, A, B; cin >> N >> A >> B;
        if (A > B)
            swap(A, B);
        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];
        sort(V.begin(), V.end());
        int answer = N;

        vector<int64_t> with_A(N, 0);
        for (int i = 0; i < N; ++i) {
            with_A[i] = (V[i] - 1) / A + 1;
            if (i > 0)
                with_A[i] += with_A[i - 1];
        }
        int last_B = 0;
        for (; last_B + 1 < N && V[last_B + 1] <= B; ++last_B);
        for (int oneshot = 0; oneshot <= N; ++oneshot) {
            last_B = min(last_B, N - oneshot - 1);
            // oneshot the most powerful ones
            // mandatory A's
            int64_t Akills = subsequence(with_A, last_B + 1, N - oneshot - 1);

            // now binsary search
            int step = 1;
            while (step <= N)
                step *= 2;
            int pos;
            for (pos = -1; step; step >>= 1) {
                if (pos + step > last_B)
                    continue;
                if (Akills + with_A[pos + step] <= last_B - (pos + step))
                    pos += step;
            }
            if (Akills + (pos >= 0 ? with_A[pos] : 0) > last_B - pos)
                continue;
            answer = min(answer, last_B - pos + oneshot);
        }
        cout << answer << "\n";
    }
}
