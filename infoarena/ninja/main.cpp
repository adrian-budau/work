#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

class FenwickTree {
  public:
    FenwickTree(int size_):
        data_(size_, 0) {
    }

    int size() const {
        return data_.size();
    }

    void add(int position, int64_t value) {
        for (; position < size(); position |= (position + 1))
            data_[position] += value;
    }

    int64_t query(int position) {
        int64_t result = 0;
        for (; position >= 0; position = ((position & (position + 1)) - 1))
            result += data_[position];
        return result;
    }

    int64_t query(int from, int to) {
        return query(to) - query(from - 1);
    }

  private:
    vector<int64_t> data_;
};

class RangeFenwickTree {
  public:
    RangeFenwickTree(int size_):
        multiplier_(size_),
        remainder_(size_) {
    }

    void add(int from, int to, int value) {
        multiplier_.add(from, value);
        remainder_.add(from, static_cast<int64_t>(-value) * (from - 1));

        multiplier_.add(to, -value);
        remainder_.add(to, static_cast<int64_t>(value) * to);
    }

    int64_t query(int position) {
        return multiplier_.query(position) * position + remainder_.query(position);
    }

  private:
    FenwickTree multiplier_, remainder_;
};

int main() {
    ifstream cin("ninja.in");
    ofstream cout("ninja.out");

    int N, M, K; cin >> N >> M >> K;

    assert(1 <= N and N <= 100000);
    assert(1 <= M and M <= 100000);
    assert(1 <= K and K <= 100000);

    vector< vector<int> > central(N), end(N);

    vector<int64_t> endAt(N);
    vector<int> partial(N, 0);

    for (int i = 0; i < M; ++i) {
        int from, to; cin >> from >> to;
        assert(1 <= from and from <= to and to <= N);
        assert((to - from) % 2 == 0);

        --from; --to;
        central[(from + to) / 2].push_back((to - from) / 2);
        end[to].push_back(from);
        partial[(from + to) / 2] += 1;
        if (to + 1 < N)
            partial[to + 1] -= 1;
    }

    int64_t now = 0;
    for (int i = 0; i < N; ++i) {
        now += partial[i];
        endAt[i] = (i > 0 ? endAt[i - 1] : 0) + now;
    }

    vector< vector< pair<int, int> > > queries(N);
    for (int i = 0; i < K; ++i) {
        int from, to; cin >> from >> to;
        assert(1 <= from and from <= to and to <= N);
        --from; --to;
        queries[to].emplace_back(from, i);
    }

    FenwickTree sumMiddle(N), countMiddle(N); // we count how many times the middle of an interval appears
                                              // we also keep the sum

    FenwickTree sumTotal(2 * N - 1), countTotal(2 * N - 1); // we count how many times (from + to) appears
                                                            // we also keep the sum

    RangeFenwickTree range(N);

    vector<int64_t> result(K);
    for (int i = 0; i < N; ++i) {
        for (auto &interval : central[i]) {
            int from = i - interval;
            int to = i + interval;

            sumMiddle.add((from + to) / 2, (from + to) / 2); // looks easy
            countMiddle.add((from + to) / 2, 1); // for now

            sumTotal.add(from + to, from + to); // way too easy
            countTotal.add(from + to, 1);
        }

        for (auto &interval : end[i]) {
            int from = interval;
            int to = i;

            sumMiddle.add((from + to) / 2, -(from + to) / 2); // we just reverse
            countMiddle.add((from + to) / 2, -1);

            sumTotal.add(from + to, -from - to);
            countTotal.add(from + to, -1);

            range.add(from, (from + to) / 2, 1);
        }

        for (auto &query: queries[i]) {
            int index, from, to;
            tie(from, index) = query;
            to = i;

            int64_t answer = 0;

            // all the ended intervals
            // just range sum
            answer += range.query(from - 1);

            // all the intervals not ended with the middle before from
            // thats count * to - sum on the interval 1 from - 1
            answer += static_cast<int64_t>(to + 1) * countMiddle.query(from - 1) - sumMiddle.query(from - 1);

            // all the intervals not ended with the middle between from and to (inclusive)
            // that's (from + to) * count - sum on the interval 2 * from from + to
            answer += static_cast<int64_t>(from + to) * countTotal.query(2 * from, from + to) - sumTotal.query(2 * from, from + to);

            // these were all bad
            result[index] = endAt[to] - answer;
        };
    }

    for (int i = 0; i < K; ++i)
        cout << result[i] << "\n";
}
