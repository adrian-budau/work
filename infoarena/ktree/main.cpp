#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

static const int kInfinite = numeric_limits<int>::max() / 3;

class Tree {
  public:
    Tree(int size):
            m_edge(size),
            m_size(size) {
    }

    void addEdge(int x, int y, int cost) {
        m_edge[x].emplace_back(y, cost);
        m_edge[y].emplace_back(x, cost);
    }

    typedef vector< vector<int> > state;
    state solve() {
        return dfs(0);
    }

  private:
    state dfs(int node, int father = -1) {
        m_size[node] = 1;

        vector<state> states;
        vector<int> costs;
        vector<int> sizes;
        for (auto &x : m_edge[node])
            if (x.first != father) {
                states.push_back(dfs(x.first, node));
                costs.push_back(x.second);
                sizes.push_back(m_size[x.first]);
                m_size[node] += m_size[x.first];
            }
        state answer(m_size[node] + 1, vector<int>(m_size[node], kInfinite));
        state next(m_size[node] + 1, vector<int>(m_size[node], kInfinite));
        answer[1][0] = 0;
        int total = 1;
        for (int i = 0; i < int(states.size()); ++i) {
            for (int j = 0; j <= m_size[node]; ++j)
                for (int k = 0; k < m_size[node]; ++k)
                    next[j][k] = kInfinite;
            for (int p = 0; p <= sizes[i]; ++p)
                for (int l = 0; l < sizes[i]; ++l) {
                    if (states[i][p][l] == kInfinite)
                        continue;
                    for (int j = 0; j <= total; ++j)
                        for (int k = 0; k < total; ++k) {
                            // we can take the edge
                            next[p + j][k + l] = min(next[p + j][k + l], answer[j][k] + states[i][p][l]);
                            // we can remove the edge
                            next[j][k + l + 1] = min(next[j][k + l + 1], answer[j][k] + states[i][p][l] + costs[i]);
                        }
                }
            total += sizes[i];
            swap(answer, next);
        }
        return answer;
    }

    vector< vector< pair<int, int> > > m_edge;

    vector<int> m_size;
};

int main() {
    ifstream cin("ktree.in");
    ofstream cout("ktree.out");

    int N, M, K; cin >> N >> M >> K;

    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int x, y, z; cin >> x >> y >> z;
        T.addEdge(x - 1, y - 1, z);
    }

    cout << T.solve()[K][M] << "\n";
}
