#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class SegmentTree {
  public:
    SegmentTree(const int &size) {
        for (size_ = 1; size_ < size; size_ *= 2);

        data_ = vector<int>(size_ * 2);
        lazy_ = vector<int>(size_ * 2);
    }

    void set(int position, int value) {
        data_[position + size_] = value;
        for (int i = (position + size_) / 2; i; i /= 2)
            data_[i] = max(data_[i * 2] + lazy_[i * 2], data_[i * 2 + 1] + lazy_[i * 2 + 1]);
    }

    void add(int from, int to, int value) {
        add(1, 0, size_, from, to, value);
    }

    int query() const {
        return data_[1] + lazy_[1];
    }

    int size() const {
        return size_;
    }

  private:
    void add(int node, int left, int right, int from, int to, int value) {
        if (from <= left and right <= to) {
            lazy_[node] += value;
            return;
        }

        int mid = (left + right) / 2;
        if (from < mid)
            add(node * 2, left, mid, from, to, value);
        if (mid < to)
            add(node * 2 + 1, mid, right, from, to, value);
        data_[node] = max(data_[node * 2] + lazy_[node * 2], data_[node * 2 + 1] + lazy_[node * 2 + 1]);
    }

    int size_;
    vector<int> data_;
    vector<int> lazy_;
};

int main() {
    int T; cin >> T;

    vector<int> D(T), M(T);
    for (int i = 0; i < T; ++i)
        cin >> D[i] >> M[i];

    int max_time = *max_element(D.begin(), D.end());
    SegmentTree S(max_time + 1);
    for (int i = 1; i < S.size(); ++i)
        S.set(i, -i);

    for (int i = 0; i < T; ++i) {
        S.add(D[i], S.size(), M[i]);
        cout << S.query() << "\n";
    }
}
