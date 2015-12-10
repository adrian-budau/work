#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <cassert>

using namespace std;

class Tree {
  public:
    Tree(int size):
            m_edges(size),
            m_size(size),
            m_father(size),
            m_marked(size),
            m_value(size) {
    }

    void addEdge(int from, int to) {
        m_edges[from].push_back(to);
        m_edges[to].push_back(from);
    }

    int divisors() {
        if (size() == 1)
            return 1;

        dfsSubTreeSize(0);

        map<int, int> of_size;
        for (int i = 0; i < size(); ++i)
            of_size[m_size[i]] = i;

        int answer = 2; // the graph of 1 node and the current graph
        for (int i = 2; i < size(); ++i)
            if (size() % i == 0 && of_size.count(i))
                if (check(of_size[i], i))
                    ++answer;
        return answer;
    }

    int size() const {
        return m_edges.size();
    }

  private:
    void dfsSubTreeSize(int node, int father = -1) {
        m_size[node] = 1;
        m_father[node] = father;
        for (auto &son : m_edges[node])
            if (son != father && !m_marked[son]) {
                dfsSubTreeSize(son, node);
                m_size[node] += m_size[son];
            }
    }

    bool check(int node, int size) {
        fill(m_marked.begin(), m_marked.end(), false);
        m_map.clear();

        dfsSubTreeSize(node, m_father[node]);
        int centroid = findCentroid(node, size);
        buildMap(centroid, m_father[node]);
        m_marked[node] = true;
        return dfsCheck(0, size);
    }

    void buildMap(int node, int limitNode, int father = -1) {
        vector<int> sons;
        for (auto &son : m_edges[node])
            if (son != father && !m_marked[son] && son != limitNode) {
                buildMap(son, limitNode, node);
                sons.push_back(m_value[son]);
            }
        sort(sons.begin(), sons.end());
        m_value[node] = value(sons);
    }

    bool dfsCheck(int node, int size) {
        m_size[node] = 1;
        for (auto &son : m_edges[node])
            if (son != m_father[node] && !m_marked[son]) {
                if (!dfsCheck(son, size))
                    return false;
                m_size[node] += m_size[son];
            }

        if (m_size[node] > size)
            return false;
        if (m_size[node] == size) {
            int centroid = findCentroid(node, size);
            int nextCentroid = m_father[centroid];

            if (dfsProbe(centroid, size, m_father[node])) {
                m_size[node] = 0;
                m_marked[node] = true;
                return true;
            }

            if (centroid != node && dfsProbe(nextCentroid, size, m_father[node])) {
                m_size[node] = 0;
                m_marked[node] = true;
                return true;
            }
            return false;
        }

        return true;
    }

    bool dfsProbe(int node, int size, int limitNode, int father = -1) {
        vector<int> sons;
        for (auto &son : m_edges[node])
            if (son != father && !m_marked[son] && son != limitNode) {
                if (!dfsProbe(son, size, limitNode, node))
                    return false;
                sons.push_back(m_value[son]);
            }
        sort(sons.begin(), sons.end());
        if ((m_value[node] = newValue(sons)) == -1)
            return false;
        return true;
    }

    int findCentroid(int node, int size) {
        bool good = true;
        for (auto &son : m_edges[node])
            if (son != m_father[node] && !m_marked[son]) {
                int find = findCentroid(son, size);
                if (find != -1)
                    return find;
                if (m_size[son] > size / 2)
                    good = false;
            }
        if (m_size[node] < (size + 1) / 2)
            good = false;
        if (good)
            return node;
        return -1;
    }

    int value(const vector<int>& v) {
        return m_map.emplace(v, m_map.size()).first->second;
    }

    int newValue(const vector<int>& v) {
        auto it = m_map.find(v);
        if (it == m_map.end())
            return -1;
        return it->second;
    }

    vector< vector<int> > m_edges;
    vector<int> m_size, m_father;

    vector<bool> m_marked;
    vector<int> m_value;
    map<vector<int>, int> m_map;
};

int main() {
    ifstream cin("divizori2.in");
    ofstream cout("divizori2.out");

    int N; cin >> N;
    assert(1 <= N && N <= 100 * 1000);
    Tree T(N);

    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        T.addEdge(x - 1, y - 1);
    }

    cout << T.divisors() << "\n";
}
