#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <cassert>
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

        set<int> X{x};
        set<int> Y{y};

        // begin, end and the father to skip
        vector<pair<set<int>::iterator, set<int>::iterator> > queueX{{X.begin(), X.end()}}, queueY{{Y.begin(), Y.end()}};
        vector<int> nodesX, nodesY;

        int startx, starty, step;
        for (startx = 0, starty = 0, step = 1; startx < int(queueX.size()) && starty < int(queueY.size()); step = step ^ 1)
            if (step) {
                auto& begin = get<0>(queueX[startx]);
                auto& end = get<1>(queueX[startx]);
                int node = *begin;
                if (++begin == end)
                    ++startx;

                if (m_used[node])
                    continue;
                nodesX.push_back(node);
                m_used[node] = true;
                if (m_edges[node].size())
                    queueX.emplace_back(m_edges[node].begin(), m_edges[node].end());
            } else {
                auto& begin = get<0>(queueY[starty]);
                auto& end = get<1>(queueY[starty]);
                int node = *begin;
                if (++begin == end)
                    ++starty;

                if (m_used[node])
                    continue;
                nodesY.push_back(node);
                m_used[node] = true;
                if (m_edges[node].size())
                    queueY.emplace_back(m_edges[node].begin(), m_edges[node].end());
            }

        if (startx == int(queueX.size()))
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

    bool connected() const {
        queue<int> Q;
        vector<bool> used(size(), false);
        Q.push(0);
        used[0] = true;
        while (!Q.empty()) {
            int x = Q.front();
            Q.pop();
            for (auto &y : m_edges[x])
                if (!used[y]) {
                    Q.push(y);
                    used[y] = true;
                }
        }
        return used == vector<bool>(size(), true);
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

    assert(2 <= N && N <= 100 * 1000);
    assert(1 <= M && M <= 500 * 1000);

    Tree T(N);
    set< pair<int, int> > S;
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        assert(1 <= x && x <= N);
        assert(1 <= y && y <= N);
        T.addEdge(x - 1, y - 1);
        if (x > y)
            swap(x, y);
        S.emplace(x, y);
    }

    assert(T.connected());

    int XOR = 0;
    for (int i = 0; i < M; ++i) {
        int type, x, y; cin >> type >> x >> y;
        x ^= XOR;
        y ^= XOR;
        assert(1 <= x && x <= N);
        assert(1 <= y && y <= N);
        if (type == 1) {
            if (x > y)
                swap(x, y);
            assert(S.count(make_pair(x, y)));
            S.erase(make_pair(x, y));
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
