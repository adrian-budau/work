#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>

using namespace std;

class Tree {
  public:
    Tree(int size):
            m_edges(size),
            m_colour(size, 0),
            m_colours(1),
            m_used(size, false) {
    }

    void addEdge(int x, int y) {
        m_edges[x].insert(y);
        m_edges[y].insert(x);
    }

    void removeEdge(int x, int y) {
        m_edges[x].erase(y);
        m_edges[y].erase(x);

        vector<int> nodesX{x}, nodesY{y};
        m_used[x] = m_used[y] = true;

        int startx, starty, step;
        for (startx = 0, starty = 0, step = 1; startx < int(nodesX.size()) && starty < int(nodesY.size()); step = 1 ^ step)
            if (step) {
                int x = nodesX[startx++];
                for (auto &y : m_edges[x])
                    if (!m_used[y]) {
                        nodesX.push_back(y);
                        m_used[y] = true;
                    }
            } else {
                int x = nodesY[starty++];
                for (auto &y : m_edges[x])
                    if (!m_used[y]) {
                        nodesY.push_back(y);
                        m_used[y] = true;
                    }
            }

        if (startx == int(nodesX.size()))
            for (auto &node : nodesX)
                m_colour[node] = m_colours;
        else
            for (auto &node : nodesY)
                m_colour[node] = m_colours;
        m_colours++;
        for (auto &x: nodesX)
            m_used[x] = false;
        for (auto &y : nodesY)
            m_used[y] = false;
    }

    bool query(int x, int y) const {
        return m_colour[x] == m_colour[y];
    }

    int size() const {
        return m_edges.size();
    }

 private:
    vector< set<int> > m_edges;
    vector<int> m_colour;
    int m_colours;
    vector<bool> m_used;
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
