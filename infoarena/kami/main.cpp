#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class SegmentTree {
  public:
    SegmentTree(int size):
        size_(size),
        data_(compute(size_), {0, 0}) {
    }

    int size() const {
        return size_;
    }

    void set(int position, int value) {
        update(1, 0, size(), position, value);
    }

    int query(int position) {
        pair<int, int64_t> current{-1, value(position)};
        if (position)
            query(1, 0, size(), position - 1, current);
        return current.first;
    }

  private:
    int compute(int size) const {
        int left = 0, right = size;
        int real_size = 1;
        while (left + 1 != right) {
            left = (left + right) / 2;
            real_size = real_size * 2 + 1;
        }
        return real_size + 1;
    }

    int64_t value(int position) {
        int left = 0, right = size();
        int node = 1;
        while (right - left != 1) {
            int mid = (left + right) / 2;
            if (position < mid) {
                right = mid;
                node *= 2;
            } else {
                left = mid;
                node = node * 2 + 1;
            }
        }
        return data_[node].sum;
    }

    void update(int node, int left, int right, int position, int value) {
        if (right - left == 1) {
            data_[node] = {value, value};
            return;
        }

        int mid = (left + right) / 2;
        if (position < mid)
            update(node * 2, left, mid, position, value);
        else
            update(node * 2 + 1, mid, right, position, value);

        data_[node].sum = data_[node * 2].sum + data_[node * 2 + 1].sum;
        data_[node].best = max(data_[node * 2 + 1].best, data_[node * 2].best - data_[node * 2 + 1].sum);
    }

    void query(int node, int left, int right, int position, pair<int, int64_t> &current) {
        if (current.first != -1)
            return;
        if (right <= position + 1) {
            if (data_[node].best - current.second >= 0) { // found you
                if (right - left == 1) // we even found the answer
                    current.first = left;
                else {
                    int mid = (left + right) / 2;
                    query(node * 2 + 1, mid, right, position, current);
                    query(node * 2, left, mid, position, current);
                }
            } else
                current.second += data_[node].sum;
            return;
        }

        int mid = (left + right) / 2;
        if (position >= mid)
            query(node * 2 + 1, mid, right, position, current);
        query(node * 2, left, mid, position, current);
    }

    struct Node {
        int64_t best, sum;
    };

    int size_;
    vector<Node> data_;
};

int main() {
    ifstream cin("kami.in");
    ofstream cout("kami.out");

    int N; cin >> N;

    SegmentTree S(N);
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        S.set(i, x);
    }

    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        int type; cin >> type;
        if (type == 0) {
            int x, value; cin >> x >> value;
            S.set(x - 1, value);
        } else {
            int x; cin >> x;
            cout << S.query(x - 1) + 1 << "\n";
        }
    }
}
