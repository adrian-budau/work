#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <cassert>

using namespace std;

class Tree {
  public:
    Tree(int size):
            m_edges(size) {
    }

    void addEdge(int from, int to, int length) {
        m_edges[from].emplace_back(to, length);
        m_edges[to].emplace_back(from, length);
    }

    int64_t shortestCover(int root) const {
        return dfs(root).second;
    }

  private:
    pair<int64_t, int64_t> dfs(int node, int father = -1, int64_t distanceToRoot = 0) const {
        vector< tuple<int, int64_t, int64_t> > data;
        data.reserve(m_edges[node].size());

        for (auto &son : m_edges[node])
            if (son.first != father) {
                auto result = dfs(son.first, node, distanceToRoot + son.second);
                data.emplace_back(son.second, result.first, result.second);
            }

        int64_t sameNode = 0;
        for (auto &t : data) {
            int edge; int64_t sonSameNode, sonAnywhere;
            tie(edge, sonSameNode, sonAnywhere) = t;
            sameNode += min(edge + sonSameNode + edge, edge + sonAnywhere + distanceToRoot);
        }

        int64_t anywhere = sameNode;
        int64_t win = 0;
        for (auto &t : data) {
            int edge; int64_t sonSameNode, sonAnywhere;
            tie(edge, sonSameNode, sonAnywhere) = t;
            int64_t bestSameNode = min(edge + sonSameNode + edge, edge + sonAnywhere + distanceToRoot);
            int64_t bestAnywhere = edge + sonAnywhere;
            win = max(win, bestSameNode - bestAnywhere);
        }

        anywhere -= win;
        return {sameNode, anywhere};
    }

    vector< vector<pair<int, int> > > m_edges;
};

int main() {
    ifstream cin("riremito.in");
    ofstream cout("riremito.out");

    int N; cin >> N;
    assert(1 <= N && N <= 100 * 1000);
    Tree T(N);

    for (int i = 1; i < N; ++i) {
        int x, y, z; cin >> x >> y >> z;
        assert(1 <= x && x <= N);
        assert(1 <= y && y <= N);
        assert(1 <= z && z <= 1000 * 1000 * 1000);
        T.addEdge(x - 1, y - 1, z);
    }

    int K; cin >> K;
    assert(1 <= K && K <= 10);
    for (int i = 0; i < K; ++i) {
        int root; cin >> root;
        assert(1 <= root && root <= N);
        cout << T.shortestCover(root - 1) << "\n";
    }
}
