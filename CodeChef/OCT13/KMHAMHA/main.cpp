#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class BipartiteGraph {
  public:
    BipartiteGraph(int left_size, int right_size):
        left_size_(left_size),
        right_size_(right_size),
        edges_(left_size) {
    }

    void addEdge(int left, int right) {
        edges_[left].push_back(right);
    }

    int minimumEdgeCover() {
        left_ = vector<int>(left_size_, -1);
        right_ = vector<int>(right_size_, -1);

        bool pairedSomething = true;
        while (pairedSomething) {
            used_ = vector<bool>(left_size_, false);

            pairedSomething = false;
            for (int i = 0; i < left_size_; ++i)
                if (left_[i] == -1)
                    pairedSomething |= pair(i);
        }

        return count_if(left_.begin(), left_.end(), [](int x) {
            return x != -1;
        });
    }

  private:
    bool pair(int node) {
        if (used_[node])
            return false;
        used_[node] = true;

        for (auto &right_node : edges_[node])
            if (right_[right_node] == -1) {
                right_[right_node] = node;
                left_[node] = right_node;
                return true;
            }

        for (auto &right_node : edges_[node])
            if (pair(right_[right_node])) {
                right_[right_node] = node;
                left_[node] = right_node;
                return true;
            }
        return false;
    }

    int left_size_, right_size_;
    vector< vector<int> > edges_;

    vector<bool> used_;
    vector<int> left_, right_;
};

int main() {
    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        vector< pair<int, int> > P(N);
        vector<int> X, Y;
        for (int i = 0; i < N; ++i) {
            cin >> P[i].first >> P[i].second;
            X.push_back(P[i].first);
            Y.push_back(P[i].second);
        }

        sort(X.begin(), X.end());
        sort(Y.begin(), Y.end());
        X.erase(unique(X.begin(), X.end()), X.end());
        Y.erase(unique(Y.begin(), Y.end()), Y.end());

        BipartiteGraph G(X.size(), Y.size());
        for (auto &point : P) {
            G.addEdge(lower_bound(X.begin(), X.end(), point.first) - X.begin(),
                      lower_bound(Y.begin(), Y.end(), point.second) - Y.begin());
        }

        cout << G.minimumEdgeCover() << "\n";
    }
}
