#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef long double double64;

int main() {
    ifstream cin("maxim2.in");
    ofstream cout("maxim2.out");

    int N; cin >> N;
    vector<int> V(N + 2);

    int missing = 0;
    vector<int> have(N + 1, 1);
    V[0] = V[N + 1] = -1;
    for (int i = 1; i <= N; ++i) {
        cin >> V[i];
        if (V[i] == 0)
            ++missing;
        have[V[i]] = 0;
    }

    for (int i = 1; i <= N; ++i)
        have[i] += have[i - 1];

    vector<int64_t> ways1(N + 1, 0), ways2(N + 1, 0);
    for (int i = 1, current = 0; i <= N; current += have[i] - have[i - 1], ++i) {
        if (have[i] == have[i - 1]) {
            ways1[i] = ways1[i - 1];
            ways2[i] = ways2[i - 1];
            continue;
        }
        ways1[i] = current + ways1[i - 1];
        ways2[i] = 1LL * current * (current - 1) + ways2[i - 1];
    }

    double64 answer = 0;
    for (int i = 1; i <= N; ++i)
        if (V[i] != 0) {
            if (V[i] <= V[i - 1] || V[i] <= V[i + 1])
                continue;
            int zeros = (V[i - 1] == 0) + (V[i + 1] == 0);
            if (zeros == 0)
                answer += 1;
            else if (zeros == 1)
                answer += double64(have[V[i] - 1]) / double64(missing);
            else
                answer += double64(have[V[i] - 1]) * double64(have[V[i] - 1] - 1) / (double64(missing) * double64(missing - 1));
        } else {
            int minv = max(V[i - 1], V[i + 1]);
            int zeros = (V[i - 1] == 0) + (V[i + 1] == 0);
            if (zeros == 0)
                answer += double64(have[N] - have[minv]) / double64(missing);
            else if (zeros == 1)
                answer += double64(ways1[N] - ways1[minv]) / (double64(missing) * double64(missing - 1));
            else
                answer += double64(ways2[N] - ways2[minv]) / (double64(missing) * double64(missing - 1)  * double64(missing - 2));
        }

    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(9);
    cout << answer << "\n";
}
