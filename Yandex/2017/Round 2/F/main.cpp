#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;

class Tree {
  public:
    Tree(int size):
        m_edges(size),
        m_down(size, 0),
        m_up(size, 0),
        m_depth1(size, 0),
        m_depth2(size, 0),
        m_powers(size + 1, 0),
        m_dup(size, 0) {
        m_powers[0] = 1;
        for (int i = 1; i <= size; ++i)
            m_powers[i] = (2 * m_powers[i - 1]) % kModulo;
    }

    void add_edge(int x, int y) {
        m_edges[x].emplace_back(y);
        m_edges[y].emplace_back(x);
    }

    int size() const {
        return m_edges.size();
    }

    vector<int> special() {
        dfs_down(0);
        m_up[0] = 0;
        m_dup[0] = 0;
        dfs_up(0);
        return m_up;
    }

  private:
    void dfs_down(int node, int father = -1) {
        int d1 = -1, d2 = -1;
        for (auto &x : m_edges[node])
            if (x != father) {
                dfs_down(x, node);
                m_down[node] = (m_down[node] + m_down[x]) % kModulo;
                if (m_depth1[x] > d1) {
                    d2 = d1;
                    d1 = m_depth1[x];
                } else if (m_depth1[x] > d2) {
                    d2 = m_depth1[x];
                }
            }
        m_depth1[node] = d1 + 1;
        m_depth2[node] = d2 + 1;
        m_down[node] = (m_down[node] + m_powers[d1 + 1]) % kModulo;
    }

    void dfs_up(int node, int father = -1) {
        for (auto &x : m_edges[node])
            if (x != father)
                m_up[node] = (m_up[node] + m_down[x]) % kModulo;
        for (auto &x : m_edges[node]) {
            if (x == father)
                continue;
            int up_depth = m_dup[node];
            if (m_depth1[x] + 1 == m_depth1[node])
                up_depth = max(up_depth, m_depth2[node]);
            else
                up_depth = max(up_depth, m_depth1[node]);
            m_up[x] = (m_up[node] - m_down[x]) % kModulo;
            m_up[x] = (m_up[x] + m_powers[up_depth]) % kModulo;
            m_dup[x] = up_depth + 1;
            dfs_up(x, node);
        }
    }

    vector< vector<int> > m_edges;

    vector<int> m_down, m_up, m_depth1, m_depth2, m_powers, m_dup;
};

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        T.add_edge(x - 1, y - 1);
    }

    auto answer = T.special();
    for (auto &x : answer) {
        if (x < 0)
            x += kModulo;
        cout << x << "\n";
    }
}
