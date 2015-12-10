#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

class Tree {
  public:
    Tree(int size):
            m_edges(size) {
    }

    void addEdge(int x, int y) {
        m_edges[x].insert(y);
        m_edges[y].insert(x);
    }

    void removeEdge(int x, int y) {
        m_edges[x].erase(y);
        m_edges[y].erase(x);
    }

    bool query(int x, int y) const {
        return dfs(x, y);
    }

    int size() const {
        return m_edges.size();
    }

  private:
    bool dfs(int node, int find, int father = -1) const {
        if (node == find)
            return true;

        for (auto &x : m_edges[node])
            if (x != father)
                if (dfs(x, find, node))
                    return true;
        return false;
    }

    vector< set<int> > m_edges;
};

int main() {
    ifstream cin("disconnect.in");
    ofstream cout("disconnect.out");

    int N, M; cin >> N >> M;

    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        T.addEdge(x - 1, y - 1);
    }

    int XOR = 0;
    for (int i = 0; i < M; ++i) {
        int type, x, y; cin >> type >> x >> y;
        x ^= XOR;
        y ^= XOR;
        if (type == 1) {
            T.removeEdge(x - 1, y - 1);
        } else
            if (T.query(x - 1, y - 1)) {
                cout << "YES\n";
                XOR = x;
            } else {
                cout << "NO\n";
                XOR = y;
            }
    }
}
