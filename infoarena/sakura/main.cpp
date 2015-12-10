#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

typedef vector< vector<int> > graph;
graph readGraph(istream& cin) {
    int size; cin >> size;

    graph edges(size);
    for (int i = 1; i < size; ++i) {
        int x, y; cin >> x >> y;
        edges[x].push_back(y);
        edges[y].push_back(x);
    }

    return edges;
}

graph normalize(const graph& G) {
    queue<int> Q;
    vector<bool> used(G.size(), false);

    graph newG(G.size());
    Q.push(0);
    used[0] = true;

    while (!Q.empty()) {
        int node = Q.front();
        Q.pop();

        for (auto &x : G[node])
            if (!used[x]) {
                newG[node].push_back(x);
                used[x] = true;
                Q.push(x);
            }
    }

    return newG;
}

class BipartiteGraph {
  public:
    BipartiteGraph(int size_left, int size_right):
        m_size_left(size_left),
        m_size_right(size_right),
        m_edges(size_left) {
    }

    void addEdge(int from, int to) {
        m_edges[from].push_back(to);
    }

    bool hasPerfectRightMatching() {
        vector<int> left(m_size_left, -1), right(m_size_right, -1);

        do {
            vector<bool> used(m_size_left, false);
            bool repeat = false;
            for (int i = 0; i < m_size_left; ++i)
                if (left[i] == -1)
                    repeat |= match(i, left, right, used);
            if (!repeat)
                break;
        } while (true);
        return count(right.begin(), right.end(), -1) == 0;
    }

  private:
    bool match(int node, vector<int> &left, vector<int> &right, vector<bool> &used) {
        if (used[node])
            return false;
        used[node] = true;

        for (auto &x : m_edges[node])
            if (right[x] == -1 || match(right[x], left, right, used)) {
                left[node] = x;
                right[x] = node;
                return true;
            }
        return false;
    }

    int m_size_left, m_size_right;
    graph m_edges;
};

bool isReductable(const graph& first, const graph& second, int firstNode = 0, int secondNode = 0, map< pair<int, int>, bool> &&memoize = map<pair<int, int>, bool>() ) {
    if (first[firstNode].size() < second[secondNode].size())
        return false;
    if (second[secondNode].empty())
        return true;

    if (memoize.count(make_pair(firstNode, secondNode)))
        return memoize[make_pair(firstNode, secondNode)];

    BipartiteGraph G(first[firstNode].size(), second[secondNode].size());
    for (int i = 0; i < int(first[firstNode].size()); ++i)
        for (int j = 0; j < int(second[secondNode].size()); ++j)
            if (isReductable(first, second, first[firstNode][i], second[secondNode][j], move(memoize)))
                G.addEdge(i, j);

    if (G.hasPerfectRightMatching())
        return (memoize[make_pair(firstNode, secondNode)] = true);

    return (memoize[make_pair(firstNode, secondNode)] = false);
}

int main() {
    ifstream cin("sakura.in");
    ofstream cout("sakura.out");

    int T; cin >> T;
    while (T--) {
        graph edgeFirst = normalize(readGraph(cin));
        graph edgeSecond = normalize(readGraph(cin));

        if (isReductable(edgeFirst, edgeSecond)) {
            cout << edgeFirst.size() - edgeSecond.size() << "\n";
        } else {
            cout << "-1\n";
        }
    }
}
