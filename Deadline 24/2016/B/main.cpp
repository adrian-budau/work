#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

class Graph {
  public:
    Graph(int size):
        m_edges(size) {
    }

    void addEdge(int from, int to, int dist) {
        m_edges[from].emplace_back(to, dist);
        m_edges[to].emplace_back(from, dist);
    }

    vector<int64_t> distances(int source) {
        vector<int64_t> D(size());

        set< pair<int64_t, int> > S;
        for (int i = 0; i < size(); ++i)
            if (i != source)
                S.emplace(D[i] = numeric_limits<int64_t>::max() / 2, i);
        S.emplace(D[source] = 0, source);

        for (int i = 0; i < size(); ++i) {
            int node = S.begin()->second;
            S.erase(S.begin());
            for (auto &e : m_edges[node])
                if (D[node] + e.second < D[e.first]) {
                    S.erase(make_pair(D[e.first], e.first));
                    D[e.first] = D[node] + e.second;
                    S.emplace(D[e.first], e.first);
                }
        }
        return D;
    }

    int size() const {
        return m_edges.size();
    }

  private:
    vector< vector<pair<int, int>> > m_edges;
};

int main() {
    ios::sync_with_stdio(false);

    int N, M; cin >> N >> M;
    Graph G(N);
    for (int i = 0; i < M; ++i) {
        int x, y, d; cin >> x >> y >> d;
        --x; --y;
        G.addEdge(x, y, d);
    }

    int T; cin >> T;

    while (T--) {
        int H, K; cin >> H >> K;
        vector<int> nodes(K + 1);
        nodes[0] = H - 1;
        for (int i = 1; i <= K; ++i) {
            cin >> nodes[i];
            --nodes[i];
        }

        ++K;
        vector< vector<int64_t> > distances(K, vector<int64_t>(K));
        for (int i = 0; i < K; ++i) {
            auto D = G.distances(nodes[i]);
            for (int j = 0; j < K; ++j)
                distances[i][j] = D[nodes[j]];
        }

        vector< vector<int64_t> > dp(K, vector<int64_t>(K));
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                dp[i][j] = numeric_limits<int64_t>::max() / 2;
        auto next = dp;
        dp[0][0] = 0;
        for (int i = 1; i < K; ++i) {
            for (int j = 0; j <= i; ++j)
                for (int k = j; k <= i; ++k)
                    next[j][k] = numeric_limits<int64_t>::max() / 2;
            for (int j = 0; j < i; ++j)
                for (int k = j; k < i; ++k) {
                    // move the first
                    next[k][i - 1] = min(next[k][i - 1], dp[j][k] + distances[j][i]);
                    // move the second
                    next[j][i - 1] = min(next[j][i - 1], dp[j][k] + distances[k][i]);
                    // move the third
                    next[j][k] = min(next[j][k], dp[j][k] + distances[i - 1][i]);
                }
            swap(dp, next);
        }

        int64_t answer = numeric_limits<int64_t>::max() / 2;
        for (int i = 0; i < K; ++i)
            for (int j = i; j < K; ++j)
                answer = min(answer, dp[i][j] + distances[i][0] + distances[j][0] + distances[K - 1][0]);
        cout << answer << "\n";
    }
}
