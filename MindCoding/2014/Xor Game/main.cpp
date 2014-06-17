#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

class XORGraph {
  public:
    XORGraph(int size):
        edges_(size),
        value_(size, 0),
        solved_(size, false) {
    }

    int size() const {
        return edges_.size();
    }

    void addEdge(int from, int to, int value) {
        edges_[from].push_back({to, value});
        edges_[to].push_back({from, value});
    }

    void solve() {
        for (int i = 0; i < size(); ++i)
            if (!solved_[i]) {
                value_[i] = 0;
                dfs(i);
            }
    }

    int operator[](const int &index) const {
        return value_[index] ^ value_[index - 1];
    }

  private:
    void dfs(int node) {
        solved_[node] = true;
        for (auto &next : edges_[node])
            if (!solved_[next.first]) {
                value_[next.first] = value_[node] ^ next.second;
                dfs(next.first);
            }
    }

    vector< vector< pair<int, int> > > edges_;

    vector<int> value_;
    vector<bool> solved_;
};

int main() {
    ios::sync_with_stdio(false);
    int N, M; cin >> N >> M;

    assert(1 <= N && N <= 100000);
    assert(1 <= M && M <= 100000);

    XORGraph G(N + 1);
    for (int i = 0; i < M; ++i) {
        int x, y, cost; cin >> x >> y >> cost;
        assert(1 <= x && x <= y && y <= N && cost >= 0 && cost < (1 << 20));
        --x;
        G.addEdge(x, y, cost);
    }

    G.solve();

    for (int i = 1; i <= N; ++i)
        cout << G[i] << " ";
    cout << "\n";
}
