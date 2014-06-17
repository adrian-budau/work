#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

class Graph {
  public:
    Graph(int size):
            size_(size),
            fact_(size + 1),
            edges_(size),
            answer_(size_ + 1, 0) {
        fact_[0] = 1;
        for (int i = 1; i <= size_; ++i)
            fact_[i] = fact_[i - 1] * i;
    }

    void addEdge(int x, int y) {
        edges_[x].push_back(y);
        edges_[y].push_back(x);
    }

    pair<int, long long> colorings() {
        colour_ = vector<int>(size_, 0);
        used_ = vector<int>(size_ + 1, 0);

        answer_so_far_ = numeric_limits<int>::max();
        backtrack(0);
        return {answer_so_far_, answer_[answer_so_far_]};
    }

  private:
    void backtrack(int node) {
        if (node == size_) {
            answer_so_far_ = *max_element(colour_.begin(), colour_.end());
            answer_[answer_so_far_] += fact_[answer_so_far_];
            return;
        }

        int i = 0;
        vector<bool> usedNow(size_ + 1, 0);
        for (auto &next : edges_[node])
            usedNow[colour_[next]] = true;

        do {
            ++i;
            if (i > answer_so_far_)
                break;
            if (usedNow[i])
                continue;
            colour_[node] = i;
            ++used_[i];
            backtrack(node + 1);
            --used_[i];
            colour_[node] = 0;
        } while (used_[i]);
    }

    int size_;
    vector<long long> fact_;
    vector< vector<int> > edges_;

    int answer_so_far_;
    vector<int> colour_;
    vector<int> used_;

    vector<long long> answer_;
};

template<class T1, class T2>
ostream& operator<<(ostream& output, const pair<T1, T2>& that) {
    output << that.first << " " << that.second;
    return output;
}

int main() {
    ifstream cin("colorare.in");
    ofstream cout("colorare.out");

    int N, M; cin >> N >> M;

    Graph G(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        G.addEdge(x - 1, y - 1);
    }

    cout << G.colorings() << "\n";
}
