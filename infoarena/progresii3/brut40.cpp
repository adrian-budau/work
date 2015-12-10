#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 9;

int main() {
    ifstream cin("progresii3.in");
    ofstream cout("progresii3.out");

    int64_t V; int N, K; cin >> V >> N >> K;

    assert(V <= 10 * 1000 * 1000);
    assert((100 <= N && N <= 150) || (V <= 100 * 1000));
    assert(K <= 30 || V <= 100 * 1000);

    vector<int64_t> A(K);
    for (int i = 0; i < K; ++i)
        cin >> A[i];

    int64_t P = V / (N - 1); P %= kModulo;
    int answer = ((V % kModulo) * P - (N - 1) * ((P * (P + 1) / 2) % kModulo)) % kModulo;

    for (int ratio = 1; ratio <= V / (N - 1); ++ratio) {
        map<int, int64_t> last;
        for (auto &x : A) {
            int64_t leftMost = x - int64_t(N - 1) * ratio;
            int64_t rightMost = x + int64_t(N - 1) * ratio;
            if (leftMost < 1)
                leftMost += int64_t((-leftMost) / ratio + 1) * ratio;
            if (rightMost > V)
                rightMost -= int64_t((rightMost - V - 1) / ratio + 1) * ratio;
            rightMost -= int64_t(N - 1) * ratio;
            if (leftMost <= rightMost) {
                leftMost /= ratio; ++leftMost;
                rightMost /= ratio; ++rightMost;
                if (last[x % ratio] < leftMost) {
                    answer = (answer - (rightMost - leftMost + 1)) % kModulo;
                    last[x % ratio] = rightMost;
                } else {
                    answer = (answer - (rightMost - last[x % ratio])) % kModulo;
                    last[x % ratio] = rightMost;
                }
            }
        }
    }

    if (answer < 0)
        answer += kModulo;
    cout << answer << "\n";
}
