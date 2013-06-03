#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
  public:
    Graph(const int &size):
            size_(size),
            edges_(size) {
    }

    void addEdge(const int &from, const int &to) {
        edges_[from].push_back(to);
        edges_[to].push_back(from);
    }

    int hubLocation() {
        used_ = vector<int>(size_, -1);
        count_ = vector<int>(size_, 0);

        answer_down_ = vector<long long>(size_, 0);
        answer_ = vector<long long>(size_, 0);

        dfsDown(0);

        answer_[0] = answer_down_[0];
        dfsTotal(0);

        return min_element(answer_.begin(), answer_.end()) - answer_.begin();
    }

  private:
    void dfsDown(int node, int from = 0) {
        used_[node] = from;
        count_[node] = 1;

        int total_sons = 0;
        answer_down_[node] = 0;
        for (vector<int>::iterator it = edges_[node].begin(); it != edges_[node].end(); ++it)
            if (used_[*it] == -1) {
                dfsDown(*it, node);
                total_sons += count_[*it];
                answer_down_[node] += answer_down_[*it];
            }

        count_[node] += total_sons;

        for (vector<int>::iterator it = edges_[node].begin(); it != edges_[node].end(); ++it)
            if (used_[*it] == node) {
                answer_down_[node] += 1LL * count_[*it] * (total_sons - count_[*it]);
                total_sons -= count_[*it];
            }
    }

    void dfsTotal(int node) {
        for (vector<int>::iterator it = edges_[node].begin(); it != edges_[node].end(); ++it)
            if (used_[*it] == node) {
                answer_[*it] = answer_[node] - answer_down_[*it] - 1LL * count_[*it] * (size_ - count_[*it] - 1);
                answer_[*it] += answer_down_[*it];
                answer_[*it] += 1LL * (count_[*it] - 1) * (size_ - count_[*it]);

                dfsTotal(*it);
            }
    }

    int size_;
    vector< vector<int> > edges_;

    vector<int> used_, count_;

    vector<long long> answer_down_;
    vector<long long> answer_;
};

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    Graph G(N);
    for (int i = 1; i < N; ++i) {
        int from, to; cin >> from >> to;
        G.addEdge(from - 1, to - 1);
    }

    cout << G.hubLocation() + 1 << "\n";
}
