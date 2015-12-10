#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

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
        vector<int> leafs = {root};
        for (int i = 0; i < size(); ++i)
            if (i != root && m_edges[i].size() == 1)
                leafs.push_back(i);

        vector< vector<int64_t> > D(leafs.size(), vector<int64_t>(leafs.size(), 0));
        for (int i = 0; i < int(leafs.size()); ++i) {
            int x = leafs[i];
            vector<int64_t> dist(size());
            dfs(x, dist);
            for (int j = 0; j < int(leafs.size()); ++j) {
                int y = leafs[j];
                D[i][j] = dist[y];
            }
        }

        for (int i = 0; i < int(leafs.size()); ++i)
            for (int j = 0; j < int(leafs.size()); ++j)
                D[i][j] = min(D[i][j], D[0][j]);

        int N = leafs.size();

        vector< vector<int64_t> > dp(1 << N, vector<int64_t>(N, numeric_limits<int64_t>::max() / 2));
        dp[1][0] = 0;
        for (int i = 0; i < (1 << N); ++i)
            for (int j = 0; j < N; ++j)
                if ((1 << j) & i)
                    for (int k = 0; k < N; ++k)
                        if (!((1 << k) & i))
                            dp[i|(1 << k)][k] = min(dp[i|(1 << k)][k], dp[i][j] + D[j][k]);
        return *min_element(dp.back().begin(), dp.back().end());
    }

  private:
    int size() const {
        return m_edges.size();
    }

    void dfs(int node, vector<int64_t>& dist, int father = -1, int64_t distanceToRoot = 0) const {
        dist[node] = distanceToRoot;
        for (auto &son : m_edges[node])
            if (son.first != father)
                dfs(son.first, dist, node, distanceToRoot + son.second);
    }

    vector< vector<pair<int, int> > > m_edges;
};

int main() {
    ifstream cin("riremito.in");
    ofstream cout("riremito.out");

    int N; cin >> N;
    Tree T(N);

    for (int i = 1; i < N; ++i) {
        int x, y, z; cin >> x >> y >> z;
        T.addEdge(x - 1, y - 1, z);
    }

    int K; cin >> K;
    for (int i = 0; i < K; ++i) {
        int root; cin >> root;
        cout << T.shortestCover(root - 1) << "\n";
    }
}

