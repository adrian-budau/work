#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int64_t B; int N; cin >> B >> N;

        vector<int64_t> V(37);
        for (int i = 0; i < N; ++i)
            cin >> V[i];
        sort(V.begin(), V.end());

        long double answer = 0;

        for (int i = 0; i < 37; ++i) {
            int64_t need = 0;
            long double profit = 0;
            for (int j = 0; j <= i; ++j) {
                need += V[i] - V[j];
                profit += 36 * static_cast<long double>(V[i] - V[j]) / static_cast<long double>(i + 1);
            }

            vector<int64_t> S(V.begin() + i + 1, V.end());
            for (auto &x: S)
                if (x == V[i]) {
                    ++need;
                    ++x;
                }

            auto sub = [&](const int64_t &value) {
                vector<int64_t> S2;
                for (auto &x : S) {
                    x -= value;
                    if (x != 0)
                        S2.push_back(x);
                }

                swap(S, S2);
            };
            sub(V[i]);

            if (need > B)
                break;

            while (S.size() and need <= B) {
                answer = max(answer, profit - need);
                if (37 - int(S.size()) + need > B)
                    break;
                if (S.front() == 1) {
                    int P = count(S.begin(), S.end(), 1);
                    if (37 - int(S.size()) + P + need > B)
                        break;
                    need += 37 - int(S.size()) + P;
                    profit += 36;
                    sub(1);
                    continue;
                }

                int64_t groups = (B - need) / (37 - S.size());
                if (groups > S.front() - 1)
                    groups = S.front() - 1;

                need += groups * int64_t(37 - S.size());
                profit += static_cast<long double>(groups) * 36;
                sub(groups);
            }
        }

        cout << "Case #" << test << ": ";
        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(15);

        cout << answer << "\n";
    }
}
