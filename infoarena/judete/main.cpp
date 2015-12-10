#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
  public:
    Graph(int size):
        m_edge(size) {
    }

    void addEdge(int x, int y) {
        m_edge[x].emplace_back(y);
        m_edge[y].emplace_back(x);
    }

    int minMaxCounty(int minimum) const {
        vector< vector<int> > dp(size(), vector<int>(size() + 1, size()));

        dfs(0, dp, minimum);
        return *min_element(dp[0].begin() + minimum, dp[0].end());
    }

    int size() const {
        return m_edge.size();
    }

  private:
    void dfs(int node, vector< vector<int> > &dp, int minimum, int father = -1) const {
        dp[node][1] = 1;
        for (auto &x : m_edge[node])
            if (x != father) {
                dfs(x, dp, minimum, node);
                int least = *min_element(dp[x].begin() + minimum, dp[x].end());
                for (int j = size(); j >= 0; --j) {
                    for (int k = 1; j + k <= size(); ++k)
                        dp[node][j + k] = min(dp[node][j + k], max({dp[node][j], dp[x][k], j + k}));
                    dp[node][j] = max(dp[node][j], least); // if we don't pick this son to join with
                }
            }
    }

    vector< vector<int> > m_edge;
};

int main() {
    ifstream cin("judete.in");
    ofstream cout("judete.out");

    int N, K; cin >> N >> K;

    Graph G(N);
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        G.addEdge(x - 1, y - 1);
    }

    cout << G.minMaxCounty(K) << "\n";
}
