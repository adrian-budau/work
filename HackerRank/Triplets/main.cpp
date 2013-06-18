#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class FenwickTree {
  public:
    FenwickTree(const int &size):
            size_(size),
            data_(size_ + 1, 0) {
    }

    void add(int where, int value) {
        for (int i = where; i <= size_; i += (i & -i))
            data_[i] += value;
    }

    int sum(int where) const {
        int answer = 0;
        for (int i = where; i > 0; i -= (i & -i))
            answer += data_[i];
        return answer;
    }

    int sum(int from, int to) const {
        return sum(to) - sum(from - 1);
    }
  private:
    int size_;
    vector<int> data_;
};

int main() {
    int N; cin >> N;

    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    vector<int> A = V;
    sort(A.begin(), A.end());
    A.erase(unique(A.begin(), A.end()), A.end());
    for (auto &x : V)
        x = lower_bound(A.begin(), A.end(), x) - A.begin() + 1;

    int max_value = *max_element(V.begin(), V.end());
    vector< vector<int> > where(max_value + 1);

    FenwickTree lower(max_value);
    vector<int> smaller(N);
    for (int i = 0; i < N; ++i) {
        if (not where[V[i]].size()) {
            where[V[i]].push_back(i);
            lower.add(V[i], 1);
        }
        smaller[i] = lower.sum(1, V[i] - 1);
    }

    where = vector< vector<int> >(max_value + 1);
    FenwickTree higher(max_value);
    vector<int> bigger(N);
    for (int i = N - 1; i >= 0; --i) {
        if (not where[V[i]].size()) {
            where[V[i]].push_back(i);
            higher.add(V[i], 1);
        } else
            where[V[i]].push_back(i);
        bigger[i] = higher.sum(V[i] + 1, max_value);
    }

    int64_t answer = 0;
    for (int i = 1; i <= max_value; ++i) {
        if (where[i].size() == 1) {
            int position = where[i][0];
            answer += static_cast<int64_t>(smaller[position]) * bigger[position];
            continue;
        }

        int first = where[i][1];
        int second = where[i][0];
        answer += static_cast<int64_t>(smaller[first]) * bigger[first];
        answer += static_cast<int64_t>(smaller[second]) * bigger[second];
        answer -= static_cast<int64_t>(smaller[first]) * bigger[second];
    }

    cout << answer << "\n";
}

