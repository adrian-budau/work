#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
#include <tuple>

using namespace std;

static const int kMaxValue = 100 * 1000;

class Graph {
  public:
    Graph(int size):
        m_edge(size),
        m_value(size) {
    }

    void addEdge(int x, int y, int cost, bool tree = false) {
        m_edge[x].emplace_back(y, cost, tree);
    }

    int size() const {
        return m_edge.size();
    }

    void solve() {
        vector<int64_t> distance(size(), numeric_limits<int64_t>::max());
        distance[0] = 0;

        queue<int> Q;
        Q.push(0);

        vector<bool> inQueue(size(), false);
        inQueue[0] = true;

        vector<int> countQueue(size(), 0);
        countQueue[0] = 1;

        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();

            inQueue[node] = false;
            for (auto &edge : m_edge[node]) {
                int next, cost;
                tie(next, cost, ignore) = edge;
                if (distance[node] + cost < distance[next]) {
                    distance[next] = distance[node] + cost;
                    if (!inQueue[next]) {
                        inQueue[next] = true;
                        countQueue[next]++;
                        Q.push(next);
                    }
                }
            }
        }

        dfsValues(0, distance);
    }

    int value(int node) const {
        return m_value[node];
    }

  private:
    void dfsValues(int node, const vector<int64_t>& distance, int father = -1) {
        for (auto &edge : m_edge[node]) {
            int next, type;
            tie(next, ignore, type) = edge;
            if (next != father && type) {
                m_value[next] = distance[next] - distance[node];
                dfsValues(next, distance, node);
            }
        }
    }

    vector< vector< tuple<int, int, bool> > > m_edge;
    vector<int> m_value;
};

int main() {
    ifstream cin("import.in");
    ofstream cout("import.out");

    int N, M, K; cin >> N >> M >> K;

    Graph G(N);
    for (int i = 1; i < N; ++i) {
        int x, y;
        cin >> x >> y;
        --x; --y;
        G.addEdge(x, y, kMaxValue, true);
        G.addEdge(y, x, kMaxValue, true);
    }

    for (int i = 0; i < M; ++i) {
        int a, b, c, d; cin >> a >> b >> c >> d;
        --a; --b;
        if (b < K)
            swap(a, b);
        if (d == 0)
            G.addEdge(a, b, -c);
        else
            G.addEdge(b, a, c - 1);
    }

    G.solve();
    for (int i = 0; i < N; ++i)
        cout << (i < K ? 1 : -1) * G.value(i) << " ";
}

