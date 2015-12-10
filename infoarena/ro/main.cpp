#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

class Graph {
  public:
    Graph(int size):
        m_edge(size),
        m_map(size, vector<bool>(size, 0)),
        m_cost(size),
        m_lowlink(size),
        m_depth(size, 0),
        m_promoted(size, 0),
        m_not_promoted(size, 0),
        m_promoted_way(size),
        m_not_promoted_way(size) {
    }

    int size() const {
        return m_edge.size();
    }

    int cost(int x) const {
        return m_cost[x];
    }

    void setCost(int where, int x) {
        m_cost[where] = x;
    }

    void addEdge(int from, int to) {
        m_edge[from].push_back(to);
        m_edge[to].push_back(from);
        m_map[from][to] = m_map[to][from] = true;
    }

    vector<int> bestPick() {
        m_depth[0] = 1;
        dfs(0);

        if (m_promoted[0] < m_not_promoted[0])
            return m_promoted_way[0];
        else
            return m_not_promoted_way[0];
    }

  private:
    void dfs(int node) {
        m_lowlink[node] = m_depth[node];
        m_stack.push_back(node);

        m_promoted[node] = m_cost[node];
        m_not_promoted[node] = 0;
        m_promoted_way[node].push_back(node);

        for (auto &x : m_edge[node])
            if (not m_depth[x]) {
                m_depth[x] = m_depth[node] + 1;
                dfs(x);
                if (m_lowlink[x] >= m_depth[node]) {
                    vector<int> component(1, node);
                    while (m_stack.back() != x) {
                        component.push_back(m_stack.back());
                        m_stack.pop_back();
                    }
                    m_stack.pop_back();
                    component.push_back(x);
                    int x, y;
                    vector<int> u, v;
                    tie(x, y, u, v) = resolve(component);

                    m_promoted[node] += x;
                    m_not_promoted[node] += y;
                    m_promoted_way[node].insert(m_promoted_way[node].end(), u.begin(), u.end());
                    m_not_promoted_way[node].insert(m_not_promoted_way[node].end(), v.begin(), v.end());
                } else {
                    m_lowlink[node] = min(m_lowlink[node], m_lowlink[x]);
                }
            } else
                m_lowlink[node] = min(m_lowlink[node], m_depth[x]);
    }

    tuple<int, int, vector<int>, vector<int>> resolve(vector<int> component) {
        int best_promoted = numeric_limits<int>::max(), best_not_promoted = numeric_limits<int>::max();
        int promoted_mask = 0, not_promoted_mask = 0;

        vector< pair<int, int> > edges;
        for (int i = 0; i < int(component.size()); ++i)
            for (auto j = i + 1; j < int(component.size()); ++j)
                if (m_map[component[i]][component[j]])
                    edges.emplace_back(i, j);

        for (int mask = 0; mask < int(1 << component.size()); ++mask) {
            bool ok = true;
            for (auto &p : edges)
                if (!((1 << p.first) & mask) && !((1 << p.second) & mask)) {
                    ok = false;
                    break;
                }
            if (!ok)
                continue;

            int aux = 0;
            for (int i = 1; i < int(component.size()); ++i)
                if (mask & (1 << i))
                    aux += m_promoted[component[i]];
                else
                    aux += m_not_promoted[component[i]];
            if (mask & 1) {
                if (aux < best_promoted) {
                    best_promoted = aux;
                    promoted_mask = mask;
                }
            } else {
                if (aux < best_not_promoted) {
                    best_not_promoted = aux;
                    not_promoted_mask = mask;
                }
            }
        }

        auto extract = [&](int mask) {
            vector<int> answer;
            for (int i = 1; i < int(component.size()); ++i)
                if ((1 << i) & mask)
                    answer.insert(answer.end(), m_promoted_way[component[i]].begin(), m_promoted_way[component[i]].end());
                else
                    answer.insert(answer.end(), m_not_promoted_way[component[i]].begin(), m_not_promoted_way[component[i]].end());
            return answer;
        };

        return make_tuple(best_promoted, best_not_promoted, extract(promoted_mask), extract(not_promoted_mask));
    }

    vector< vector<int> > m_edge;
    vector< vector<bool> > m_map;
    vector<int> m_cost;


    vector<int> m_lowlink, m_depth, m_stack;
    vector<int> m_promoted, m_not_promoted;

    vector< vector<int> > m_promoted_way, m_not_promoted_way;
};

int main() {
    ifstream cin("ro.in");
    ofstream cout("ro.out");

    int N, M; cin >> N >> M;
    Graph G(N);

    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        G.setCost(i, x);
    }

    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        G.addEdge(x - 1, y - 1);
    }

    auto answer = G.bestPick();
    int total = 0;
    for (auto &x : answer)
        total += G.cost(x);

    cout << total << "\n";
    cout << answer.size() << "\n";

    sort(answer.begin(), answer.end());
    for (auto &x : answer)
        cout << x + 1 << " ";
    cout << "\n";
}

