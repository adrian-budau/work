#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cassert>

using namespace std;

class Graph {
  public:
    Graph(int size):
            m_edges(size) {
    }

    void addEdge(int x, int y) {
        m_edges[x].push_back(y);
        m_edges[y].push_back(x);
    }

    int size() const {
        return m_edges.size();
    }

    vector<int> victoryNodes() {
        m_victory = m_special = vector<bool>(size(), false);
        m_depth = m_link = vector<int>(size(), 0);

        for (int i = 0; i < size(); ++i)
            if (not m_depth[i])
                biconnected_components(i, 1);

        vector<int> answer;
        for (int i = 0; i < size(); ++i)
            if (m_victory[i])
                answer.push_back(i);
        return answer;
    }

  private:
    void biconnected_components(int node, int depth, int father = -1) {
        m_stack.push(node);
        assert(!m_depth[node]);
        m_depth[node] = m_link[node] = depth;

        for (auto &neighbour : m_edges[node]) {
            if (neighbour == father || neighbour == node)
                continue;
            if (not m_depth[neighbour]) {
                biconnected_components(neighbour, depth + 1, node);
                m_link[node] = min(m_link[node], m_link[neighbour]);
                if (m_link[neighbour] >= m_depth[node]) {
                    bool bipartite = true;
                    vector<int> nodes;
                    int x;
                    do {
                        x = m_stack.top();
                        m_stack.pop();
                        if (m_special[x])
                            bipartite = false;
                        nodes.push_back(x);
                    } while (x != neighbour);
                    if (!bipartite) {
                        for (auto &x : nodes)
                            m_victory[x] = true;
                        m_victory[node] = true;
                    }
                }
            } else {
                m_link[node] = min(m_link[node], m_depth[neighbour]);
                if (m_depth[neighbour] < m_depth[node] && (m_depth[node] + m_depth[neighbour]) % 2 == 0)
                    m_special[node] = true;
            }
        }
    }

    vector< vector<int> > m_edges;

    vector<int> m_depth, m_link;
    stack<int> m_stack;
    vector<bool> m_special;
    vector<bool> m_victory;
};

int main() {
    ifstream cin("victorie.in");
    ofstream cout("victorie.out");

    int N, M; cin >> N >> M;
    assert(1 <= N && N <= 100 * 1000);
    assert(0 <= M && M <= 300 * 1000);

    Graph G(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        assert(1 <= x && x <= N);
        assert(1 <= y && y <= N);
        G.addEdge(x - 1, y - 1);
    }

    auto answer = G.victoryNodes();
    cout << answer.size() << "\n";
    for (auto &x : answer)
        cout << x + 1 << " ";
    cout << "\n";
}
