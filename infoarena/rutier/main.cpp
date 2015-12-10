#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

struct Edge {
    Edge(int _from, int _to, int _cost, int _index):
        from(_from), to(_to), cost(_cost), index(_index), mark(false) {}

    int from, to, cost, index;

    bool mark;
    bool operator<(const Edge& that) const {
        return cost < that.cost;
    }
};

class DisjointSet {
  public:
    DisjointSet(int size):
        m_set(size) {}

    void reset(int size) {
        for (int i = 0; i < size; ++i)
            m_set[i] = i;
    }

    int set(int x) {
        if (m_set[x] == x)
            return x;
        return (m_set[x] = set(m_set[x]));
    }

    void join(int x, int y) {
        m_set[set(x)] = set(y);
    }

  private:
    vector<int> m_set;
};

class Solver {
  public:
    Solver(int size):
            m_size(size),
            m_sets(size),
            m_contracted(size) {}

    vector<int64_t> solve() {
        m_answer = 0;
        for (auto &e : m_edges)
            m_answer += e.cost;

        sort(m_edges.begin(), m_edges.end());
        sort(m_new_edges.begin(), m_new_edges.end());
        solve(m_size, 0, m_new_edges.begin(), m_new_edges.end());
        reverse(m_answers.begin(), m_answers.end());

        m_answers[0] += m_answer;
        for (int i = 1; i < int(m_answers.size()); ++i)
            m_answers[i] += m_answers[i - 1];
        return m_answers;
    }

    void setEdges(vector<Edge> edges) {
        m_edges = edges;
    }

    void setNewEdges(vector<Edge> newEdges) {
        m_new_edges = newEdges;
    }

  private:
    template<class T>
    void solve(int vertices, int edgeBegin, T begin, T end) {
        // reduction, contraction, reduction
        reduction(vertices, edgeBegin, begin, end);
        int newVertices, newEdgeBegin;
        tie(newVertices, newEdgeBegin) = contraction(vertices, edgeBegin);
        reduction(newVertices, newEdgeBegin, begin, end, true);

        for (auto it = begin; it != end; ++it) {
            it->from = m_contracted[it->from];
            it->to = m_contracted[it->to];
        }

        if (end - begin == 1) {
            if (begin->from != begin->to && begin->cost < m_edges[newEdgeBegin].cost) {
                m_answers.push_back(-m_edges[newEdgeBegin].cost + begin->cost);
            } else
                m_answers.push_back(0);
            m_edges.erase(m_edges.begin() + newEdgeBegin, m_edges.end());
            return;
        }

        int min_index = begin->index;
        for (auto it = begin; it != end; ++it)
            min_index = min(min_index, it->index);

        int dist = end - begin;

        int newNewEdgeBegin = m_edges.size();

        for (auto it = begin; it != end; ++it)
            if (it->index < min_index + dist / 2 && it->from != it->to)
                m_edges.emplace_back(*it);

        inplace_merge(m_edges.begin() + newEdgeBegin, m_edges.begin() + newNewEdgeBegin, m_edges.end());

        stable_partition(begin, end, [&](const Edge& e) {
            return e.index < min_index + dist / 2;
        });

        solve(newVertices, newEdgeBegin, begin + dist / 2, end);
        m_edges.erase(remove_if(m_edges.begin() + newEdgeBegin, m_edges.end(), [&](const Edge& e) {
            return e.index >= min_index;
        }), m_edges.end());
        solve(newVertices, newEdgeBegin, begin, begin + dist / 2);
        m_edges.erase(m_edges.begin() + newEdgeBegin, m_edges.end());
    }

    template<class T>
    void reduction(int vertices, int edgeBegin, T begin, T end, bool erase = false) {
        m_sets.reset(vertices);
        auto it = begin;
        for (int i = edgeBegin; i < int(m_edges.size()); ++i) {
            while (!erase && it != end && it->cost < m_edges[i].cost) {
                m_sets.join(it->from, it->to);
                ++it;
            }
            if (m_sets.set(m_edges[i].from) == m_sets.set(m_edges[i].to))
                // mark as not used
                m_edges[i].mark = false;
            else {
                // mark as used
                m_sets.join(m_edges[i].from, m_edges[i].to);
                m_edges[i].mark = true;
            }
        }

        if (erase)
            m_edges.erase(remove_if(m_edges.begin() + edgeBegin, m_edges.end(), [&](const Edge& e) {
                return !e.mark;
            }), m_edges.end());
    }

    pair<int, int> contraction(int vertices, int edgeBegin) {
        m_sets.reset(vertices);
        for (int i = edgeBegin; i < int(m_edges.size()); ++i)
            if (m_edges[i].mark)
                m_sets.join(m_edges[i].from, m_edges[i].to);

        int newVertices = 0;
        for (int i = 0; i < vertices; ++i)
            if (m_sets.set(i) == i)
                m_contracted[i] = newVertices++;

        for (int i = 0; i < vertices; ++i)
            m_contracted[i] = m_contracted[m_sets.set(i)];

        int newEdgeBegin = m_edges.size();
        for (int i = edgeBegin; i < newEdgeBegin; ++i)
            if (m_contracted[m_edges[i].from] != m_contracted[m_edges[i].to]) {
                int from = m_contracted[m_edges[i].from];
                int to = m_contracted[m_edges[i].to];
                m_edges.emplace_back(from, to, m_edges[i].cost, 0);
            }
        return make_pair(newVertices, newEdgeBegin);
    }

    vector<Edge> m_edges;
    vector<Edge> m_new_edges;
    int m_size;
    DisjointSet m_sets;
    vector<int> m_contracted;

    int64_t m_answer;
    vector<int64_t> m_answers;
};

int main() {
    ifstream cin("rutier.in");
    ofstream cout("rutier.out");

    int N; cin >> N;

    vector<Edge> edges;
    int64_t answer = 0;
    for (int i = 1; i < N; ++i) {
        int from, cost; cin >> from >> cost;
        edges.emplace_back(from - 1, i, cost, i);
        answer += cost;
    }

    int M; cin >> M;
    vector<Edge> newEdges;
    for (int i = 0; i < M; ++i) {
        int from, to, cost; cin >> from >> to >> cost;
        newEdges.emplace_back(from - 1, to - 1, cost, M + i);
    }

    Solver S(N);
    S.setEdges(edges);
    S.setNewEdges(newEdges);
    for (auto &v : S.solve())
        cout << v << "\n";
}
