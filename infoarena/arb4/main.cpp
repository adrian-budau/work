#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

class DisjointSet {
  public:
    DisjointSet(int size):
            m_data(size) {
        for (int i = 0; i < size; ++i)
            m_data[i] = i;
    }

    int set(int x) {
        if (x == m_data[x])
            return x;
        return (m_data[x] = set(m_data[x]));
    }

    void join(int x, int y) {
        m_data[set(x)] = set(y);
    }

  private:
    vector<int> m_data;
};

class Graph {
  public:
    Graph(int size):
            m_edges(size),
            m_answer(size - 1, -1),
            m_depth(size, 0),
            m_father(size),
            m_edge(size),
            m_top(size),
            m_set(size) {
    }

    void addEdge(int from, int to, int index) {
        m_edges[from].emplace_back(to, index);
        m_edges[to].emplace_back(from, index);
    }

    void prepare() {
        m_depth[0] = 0;
        m_father[0] = -1;
        m_edge[0] = -1;
        dfs(0);
        for (int i = 0; i < int(m_edges.size()); ++i)
            m_top[i] = i;
    }

    int answer(int edge) const {
        return m_answer[edge];
    }

    void update(int from, int to, int index) {
        while (from != to) {
            from = m_top[m_set.set(from)];
            to = m_top[m_set.set(to)];
            if (from == to)
                return;
            if (m_depth[from] >= m_depth[to]) {
                m_answer[m_edge[from]] = index;
                join(from, m_father[from]);
                from = m_father[from];
            } else {
                m_answer[m_edge[to]] = index;
                join(to, m_father[to]);
                to = m_father[to];
            }
        }
    }

  private:
    void join(int x, int y) {
        int first = m_top[m_set.set(x)], second = m_top[m_set.set(y)];
        if (m_depth[first] > m_depth[second])
            swap(first, second);
        m_set.join(x, y);
        m_top[m_set.set(x)] = first;
    }

    void dfs(int node, int father = -1) {
        for (auto &e : m_edges[node])
            if (e.first != father) {
                m_depth[e.first] = m_depth[node] + 1;
                m_father[e.first] = node;
                m_edge[e.first] = e.second;
                dfs(e.first, node);
            }
    }

    vector< vector< pair<int, int> > > m_edges;
    vector<int> m_answer;
    vector<int> m_depth, m_father, m_edge;

    vector<int> m_top;
    DisjointSet m_set;
};

int main() {
    ifstream cin("arb4.in");
    ofstream cout("arb4.out");

    int N, M; cin >> N >> M;
    Graph G(N);
    for (int i = 1; i < N; ++i) {
        int x, y, z; cin >> x >> y >> z;
        G.addEdge(x, y, i - 1);
    }

    G.prepare();
    vector< tuple<int, int, int, int> > E(M);
    for (int i = N - 1; i < M; ++i) {
        int x, y, z; cin >> x >> y >> z;
        E[i] = make_tuple(z, x, y, i);
    }

    sort(E.begin(), E.end());
    for (auto &e : E) {
        int from, to, cost, index;
        tie(cost, from, to, index) = e;
        G.update(from, to, index);
    }

    for (int i = 0; i < N - 1; ++i)
        cout << G.answer(i) << "\n";
}
