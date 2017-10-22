#include <iostream>
#include <vector>

using namespace std;

class SegmentTree {
  public:
    SegmentTree(const vector<int>& input) {
        size_ = 1;
        while (size_ < input.size())
            size_ *= 2;
        data_ = new int[size_ * 2];
 
        for (size_t i = 0; i < input.size(); ++i)
            data_[size_ + i] = input[i];
        for (size_t i = input.size(); i < size_; ++i)
            data_[size_ + i] = 0;
         
        for (size_t i = size_ - 1; i; --i)
            data_[i] = max(data_[i * 2], data_[i * 2 + 1]);
    }
 
    int query(int from, int to) {
        int answer = 0;
        from += size_;
        to += size_;
        while (from <= to) {
            answer = max(answer, data_[from]);
            from = (from + 1) / 2;
 
            answer = max(answer, data_[to]);
            to = (to - 1) / 2;
        }
 
        return answer;
    }
 
    ~SegmentTree() {
        delete[] data_;
    }
 
  private:
    size_t size_;
    int* data_;
};

int main() {
    ios::sync_with_stdio(false);

    int N; cin >> N;
    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];
    SegmentTree S(V);

    int64_t answer = 1LL * V[0] + 1LL * V[1];
    for (int i = 0; i < N; ++i)
        for (int j = 0; i + (1 << j) < N; ++j) {
            answer = max(answer, 1LL * V[i] + 1LL * S.query(i + (1 << j), min(i + (1 << (j + 1)) - 1, N - 1)) - j);
        }
    cout << answer << "\n";
}

