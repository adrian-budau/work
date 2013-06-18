#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int64_t B; int N; cin >> B >> N;

        vector<int64_t> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];
        sort(V.begin(), V.end());

        auto sub = [&](const int64_t &value) {
            vector<int64_t> V2;
            for (int i = 0; i < N; ++i) {
                V[i] -= value;
                if (V[i] != 0)
                    V2.push_back(V[i]);
            }

            swap(V, V2);
            N = V.size();
            assert(is_sorted(V.begin(), V.end()));
        };

        if (N == 37) {
            auto aux = *min_element(V.begin(), V.end());
            sub(aux);
        }

        long double profit = 0;
        long double answer = 0;
        while (N > 0) {
            cerr << B << " " << N << "\n";
            if (B < 37 - N)
                break;
            if (V.front() == 1) {
                int P = count(V.begin(), V.end(), 1);
                for (int i = 1; i <= P and i + 37 - N <= B; ++i) {
                    long double aux_profit = profit;
                    aux_profit += static_cast<long double>(36) * static_cast<long double>(37 - N) / static_cast<long double>(37 - N + P - i) - static_cast<long double>(37 - N + i);
                    cerr << " -> " << aux_profit << "\n";
                    answer = max(answer, aux_profit);
                }

                profit += (37 - N) * (static_cast<long double>(36) / static_cast<long double>(37 - N + P) - 1);
                B -= 37 - N;
                sub(1);
                answer = max(answer, profit);
                continue;
            }

            auto now = V.front();
            int64_t groups = B / (37 - N);
            if (groups > now - 1)
                groups = now - 1;
            B -= groups * (37 - N);
            sub(groups);

            profit += static_cast<long double>(groups) * static_cast<long double>(37 - N) * (static_cast<long double>(36)/ static_cast<long double>(37 - N) - 1);
            answer = max(answer, profit);
        }

        cout << "Case #" << test << ": ";
        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(15);

        cout << answer << "\n";
    }
}
