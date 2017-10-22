#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <array>
#include <algorithm>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;
static const int kMaxCount = 100 * 1000;

int main() {
    ifstream cin("qnp.in");
    ofstream cout("qnp.out");

    int M; cin >> M;
    assert(1 <= M && M <= 100000);

    while (M--) {
        array<int, 10> count = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        int total = 0;
        for (int i = 0; i < 10; ++i) {
            cin >> count[i];
            assert(0 <= count[i] && count[i] <= kMaxCount);
            total += count[i];
        }

        int64_t K;
        cin >> K;
        assert(1 <= K && K <= 1000LL * 1000 * 1000 * 1000 * 1000);
        --K;

        string digits;
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < count[i]; ++j)
                digits.push_back('0' + i);

        while (K--)
            next_permutation(digits.begin(), digits.end());
        int answer = 0;
        for (auto &c : digits)
            answer = (10LL * answer + c - '0') % kModulo;
        cout << answer << "\n";
    }
}
