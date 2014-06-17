#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

const long long kInfinite = (1LL << 60);

class SegmentTree {
  public:
    SegmentTree(int size = 1):
            size_(size) {
        int left = 0, right = size;
        int data_size = 1;
        while (left + 1 != right) {
            left = (left + right) / 2;
            data_size = data_size * 2 + 1;
        }
        ++data_size;
        data_.resize(data_size);
        build(1, 0, size);
    }

    void set(int position, long long value) {
        add(1, 0, size_, position, position + 1, value);
    }

    void add(int from, int to, long long value) {
        add(1, 0, size_, from, to, value);
    }

    long long min() const {
        return data_[1].min;
    }

    int where() const {
        return data_[1].where;
    }

  private:
    struct Node {
        int where;
        long long min, add;

        bool operator<(const Node& that) const {
            if (min != that.min)
                return min < that.min;
            return where < that.where;
        }
    };

    void build(int node, int left, int right) {
        if (left + 1 == right) {
            data_[node] = {left, 0, 0};
            return;
        }

        int mid = (left + right) / 2;
        build(node * 2, left, mid);
        build(node * 2 + 1, mid, right);
        data_[node] = std::min(data_[node * 2], data_[node * 2 + 1]);
    }

    void add(int node, int left, int right, int from, int to, long long value) {
        if (from <= left and right <= to) {
            data_[node].min += value;
            data_[node].add += value;
            return;
        }

        int mid = (left + right) / 2;
        if (from < mid)
            add(node * 2, left, mid, from, to, value);
        if (mid < to)
            add(node * 2 + 1, mid, right, from, to, value);

        int lazy = data_[node].add;
        data_[node] = std::min(data_[node * 2], data_[node * 2 + 1]);
        data_[node].add = lazy;
        data_[node].min += lazy;
    }

    int size_;

    vector<Node> data_;
};

int main() {
    ifstream cin("biscuiti.in");
    ofstream cout("biscuiti.out");

    int N; cin >> N;

    SegmentTree S(N);
    long long answer = 0;
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        answer -= x;
        S.set(i, x);
    }

    for (int i = 0; i < N; ++i) {
        answer += S.min();
        int where = S.where();
        S.set(where, kInfinite);
        S.add(0, where + 1, i + 1);
    }

    cout << answer << "\n";
}
