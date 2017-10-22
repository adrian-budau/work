#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <array>
#include <limits>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;
static const int kMaxCount = 100 * 1000;
static const int64_t kInfinite = numeric_limits<int64_t>::max() / 4;

int main() {
    ifstream cin("qnp.in");
    ofstream cout("qnp.out");

    int M; cin >> M;
    assert(1 <= M && M <= 100000);

    vector<int> ones(kMaxCount + 1, 0);
    ones[1] = 1;
    for (int i = 2; i <= kMaxCount; ++i)
        ones[i] = (10LL * ones[i - 1]) % kModulo;

    vector< vector<int64_t> > combinations(kMaxCount + 1);
    combinations[0].push_back(1);
    for (int i = 1; i <= kMaxCount; ++i) {
        combinations[i].push_back(1);
        for (int j = 1; j <= i && j < int(combinations[i - 1].size()); ++j) {
            int64_t v = min(combinations[i - 1][j - 1] + combinations[i - 1][j], kInfinite);
            if (v < kInfinite)
                combinations[i].push_back(v);
            else
                break;
        }
        if (int(combinations[i].size()) == i)
            combinations[i].push_back(1);
    }
    for (auto& line : combinations)
        for (auto &x : line)
            assert(1 <= x && x <= kInfinite);

    auto expand = [&](int64_t count, int old, int extra) {
        // we want count * Combinations(old + extra, old)
        // and default to numeric_limits<int64_t>::max() if it overflows..
        if (extra > old)
            swap(extra, old);
        if (extra >= int(combinations[old + extra].size()))
            return kInfinite;
        int64_t multiplier = combinations[old + extra][extra];
        if (count > kInfinite / multiplier)
            return kInfinite;
        return count * multiplier;
    };

    while (M--) {
        array<int, 10> count = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
        int total = 0;
        for (int i = 0; i < 10; ++i) {
            cin >> count[i];
            assert(0 <= count[i] && count[i] <= kMaxCount);
            total += count[i];
        }
        assert(total <= kMaxCount);

        int64_t K; cin >> K;
        assert(1 <= K && K <= 1000LL * 1000 * 1000 * 1000 * 1000);
        --K;

        int answer = 0;
        while (total > 0) {
            bool found = false;
            for (int i = 0; i < 10; ++i)
                if (count[i] > 0) {
                    int64_t current_count = 1;
                    int current_total = 0;
                    for (int j = 0; j < 10; ++j) {
                        int extra = count[j];
                        if (i == j) {
                            extra--;
                        }
                        if (extra == 0)
                            continue;

                        current_count = expand(current_count, current_total, extra);
                        current_total += extra;
                        if (current_count > K)
                            break;
                    }
                    if (current_count > K) {
                        answer = (answer + 1LL * ones[total--] * i) % kModulo;
                        --count[i];
                        found = true;
                        break;
                    } else {
                        K -= current_count;
                        assert(K >= 0);
                    }
                }
            assert(found);
        }
        assert(K == 0);
        cout << answer << "\n";
    }
}
