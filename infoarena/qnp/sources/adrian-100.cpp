#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <array>
#include <limits>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;
static const int kMaxCount = 70 * 1000;
static const int64_t kMaxK = 1000LL * 1000 * 1000 * 1000;
static const int64_t kInfinite = numeric_limits<int64_t>::max() / 2;

int64_t mul(int64_t a, int64_t b) {
    if (a > kInfinite / b)
        return kInfinite;
    return a * b;
}

int64_t add(int64_t a, int64_t b) {
    if (a > kInfinite - b)
        return kInfinite;
    return a + b;
}

int main() {
    ifstream cin("qnp.in");
    ofstream cout("qnp.out");

    int M; cin >> M;
    assert(1 <= M && M <= 5000);

    vector<int> ones(kMaxCount + 1, 0);
    for (int i = 1; i <= kMaxCount; ++i)
        ones[i] = (10LL * ones[i - 1] + 1) % kModulo;

    vector< vector< vector<int64_t> > > combinations(kMaxCount + 1);
    int states = 0;
    for (int i = 0; i <= kMaxCount; ++i) {
        int64_t partial = 1;
        for (int j = 0; i + j <= kMaxCount; ++j, partial = partial * (i + j) / j) {
            if (partial > kMaxK)
                break;
            combinations[i].resize(j + 1);
            int64_t total = partial;
            for (int k = 0; i + j + k <= kMaxCount; ++k, total = total * (i + j + k) / k) {
                if (total > kMaxK)
                    break;
                int64_t aux = total;
                if (k > 0)
                    aux += combinations[i][j].back();
                combinations[i][j].push_back(aux);
                ++states;
            }
        }
    }

    auto expand = [&](int64_t count, int old, int extra) {
        // we want count * Combinations(old + extra, old)
        // and default to numeric_limits<int64_t>::max() if it overflows..
        if (extra > old)
            swap(extra, old);
        if (extra >= int(combinations[old].size()))
            return kInfinite;
        return mul(combinations[old][extra][0], count);
    };

    auto many = [&](const array<int, 10>& count) {
        int64_t current_count = 1;
        int current_total = 0;
        for (int j = 0; j < 10; ++j)
            if (count[j] > 0) {
                current_count = expand(current_count, current_total, count[j]);
                current_total += count[j];
            }
        return current_count;
    };

    auto partial = [&](int X, int Y, int Zbegin, int Zend) {
        if (Y >= int(combinations[X].size()) || Zend >= int(combinations[X][Y].size()))
            return kInfinite;
        int64_t current = combinations[X][Y][Zend];
        if (Zbegin > 0)
            current -= combinations[X][Y][Zbegin - 1];
        return current;
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
        assert(1 <= K && K <= kMaxK);
        --K;

        int answer = 0;
        while (total > 0) {
            int first_digit = 0;
            int64_t partial_K = K;
            for (int i = 0; i < 10; ++i) {
                if (count[i] > 0) {
                    --count[i];
                    int64_t c = many(count);
                    ++count[i];
                    if (c > partial_K) {
                        first_digit = i;
                        break;
                    } else
                        partial_K -= c;
                }
            }

            // We want to binary search how many we put, but this is gonna be hard
            // how the heck are we even gonna count how many numbers are with at most this prefix
            // let's simplify, it with 012 only
            int smaller_count = 0;
            int64_t smaller_ways = 1;
            for (int i = 0; i < first_digit; ++i) {
                smaller_ways = expand(smaller_ways, smaller_count, count[i]);
                smaller_count += count[i];
            }
            int larger_count = 0;
            int64_t larger_ways = 1;
            for (int i = first_digit + 1; i < 10; ++i) {
                larger_ways = expand(larger_ways, larger_count, count[i]);
                larger_count += count[i];
            }

            // ok, now let's binary search
            int step;
            for (step = 1; step < count[first_digit]; step <<= 1);
            int put;
            if (smaller_count == 0) {
                for (put = 0; step; step >>= 1) {
                    if (put + step > count[first_digit])
                        continue;
                    put += step;
                    int64_t aux = partial(0, larger_count, count[first_digit] - put, count[first_digit] - put);
                    aux = mul(aux, larger_ways);
                    if (aux <= K)
                        put -=step;
                }
            } else {
                // if we put 'em all, is it enough?
                int64_t aux = partial(smaller_count - 1, larger_count, 1, count[first_digit]);
                aux = add(aux, partial(smaller_count, larger_count, 0, 0));
                aux = mul(aux, smaller_ways);
                aux = mul(aux, larger_ways);

                if (aux > K) { // so it's some 1's followed by a 0
                    for (put = 0; step; step >>= 1) {
                        if (put + step > count[first_digit])
                            continue;
                        put += step;
                        aux = partial(smaller_count - 1, larger_count, count[first_digit] - put + 1, count[first_digit]);
                        aux = mul(aux, smaller_ways);
                        aux = mul(aux, larger_ways);

                        if (aux > K)
                            put -= step;
                    }
                } else { // some 1's followe by a 2
                    for (put = 0; step; step >>= 1) {
                        if (put + step > count[first_digit])
                            continue;
                        put += step;
                        aux = partial(smaller_count - 1, larger_count, count[first_digit] - put + 1, count[first_digit]);
                        aux = add(aux, partial(smaller_count, larger_count, count[first_digit] - put, count[first_digit] - put));
                        aux = mul(aux, smaller_ways);
                        aux = mul(aux, larger_ways);

                        if (aux <= K)
                            put -= step;
                    }
                }
                aux = partial(smaller_count - 1, larger_count, count[first_digit] - put + 1, count[first_digit]);
                aux = mul(aux, smaller_ways);
                aux = mul(aux, larger_ways);
                K -= aux;
            }

            assert(put <= count[first_digit]);
            answer = (answer + 1LL * (ones[total] - ones[total - put]) * first_digit) % kModulo;
            if (answer < 0)
                answer += kModulo;
            total -= put;
            count[first_digit] -= put;
        }
        cout << answer << "\n";
    }
}
