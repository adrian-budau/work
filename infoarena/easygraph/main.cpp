#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

class Graph {
  public:
    Graph(int size = 1):
        edges_(size),
        value_(size) {}

    void addEdge(int x, int y) {
        edges_[x].push_back(y);
    }

    void setValue(int x, int value) {
        value_[x] = value;
    }

    int size() const {
        return edges_.size();
    }

    int64_t maxCost() {
        solved_= vector<int>(size(), 0);
        for (int i = 0; i < size(); ++i)
            if (not solved_[i])
                dfs(i);

        vector<int64_t> dp(size(), 0);
        int64_t answer = numeric_limits<int64_t>::min();
        for (auto &node : search) {
            dp[node] = value_[node];
            for (auto &to : edges_[node]) {
                dp[node] = max(dp[node], dp[to] + value_[node]);
            }
            answer = max(answer, dp[node]);
        }

        return answer;
    }

  private:
    void dfs(int node) {
        solved_[node] = 1;

        for (auto &next : edges_[node])
            if (not solved_[next])
                dfs(next);
            else
                assert(solved_[next] == 2);

        search.push_back(node);
        solved_[node] = 2;
    }

    vector< vector<int> > edges_;
    vector<int> value_;

    vector<int> solved_;
    vector<int> search;
};

int main() {
    ifstream cin("easygraph.in");
    ofstream cout("easygraph.out");

    int T; cin >> T;

    while (T--) {
        int N, M; cin >> N >> M;

        Graph G(N);
        for (int i = 0; i < N; ++i) {
            int value; cin >> value;
            G.setValue(i, value);
        }

        for (int i = 0; i < M; ++i) {
            int x, y; cin >> x >> y;
            G.addEdge(x - 1, y - 1);
        }

        cout << G.maxCost() << "\n";
    }
}
