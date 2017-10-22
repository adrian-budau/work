#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

class Automata {
  public:
    Automata(int size, int sigma):
        m_edges(size, vector<int>(sigma)),
        m_final(size, false),
        m_direct(size),
        m_reverse(size) {
    }

    void set_final(int x) {
        m_final[x] = true;
    }

    bool is_final(int x) const {
        return m_final[x];
    }

    void set_edge(int from, int ch, int to) {
        m_edges[from][ch] = to;
        m_direct[from].push_back(to);
        m_reverse[to].push_back(from);
    }

    int size() const {
        return m_edges.size();
    }

    int sigma() const {
        return m_edges[0].size();
    }

    bool tolerates() const {
        vector<int> order;
        vector<bool> used(size(), false);
        for (int i = 0; i < size(); ++i)
            dfs(i, m_direct, used, order); 
        used.assign(size(), false);
        vector<int> component(size(), 0);
        int components = 0;
        reverse(order.begin(), order.end());
        for (auto &x : order) {
            vector<int> now;
            dfs(x, m_reverse, used, now);
            for (auto &node : now)
                component[node] = components;
            ++components;
        }

        vector<int> component_mask(components, 0);
        for (int i = 0; i < size(); ++i) {
            for (int j = 0; j < sigma(); ++j)
                if (component[m_edges[i][j]] == component[i])
                    component_mask[component[i]] |= (1 << j);
        }

        vector<bool> from_source(size(), false), from_destination(size(), false);
        dfs(0, m_direct, from_source, order);
        for (int i = 0; i < size(); ++i)
            if (is_final(i))
                dfs(i, m_reverse, from_destination, order);
        for (int i = 0; i < size(); ++i) {
            if (component_mask[component[i]] == (1 << sigma()) - 1 && from_source[i] && from_destination[i])
                return true;
        }
        return false;
    }

  private:
    void dfs(int node, const vector< vector<int> > &edges, vector<bool> &used, vector<int>& order) const {
        if (used[node])
            return;
        used[node] = true;
        for (auto &x : edges[node])
            dfs(x, edges, used, order);
        order.push_back(node);
    }

    vector< vector<int> > m_edges;
    vector<bool> m_final;
    vector< vector<int> > m_direct, m_reverse;
};

void test(istream& cin, ostream& cout) {
    int N, SIGMA, F; assert(cin >> N >> SIGMA >> F);
    assert(1 <= N && N <= 10 * 1000);
    assert(1 <= SIGMA && SIGMA <= 26);
    assert(1 <= F && F <= N);
    Automata A(N, SIGMA);
    for (int i = 0; i < F; ++i) {
        int x; assert(cin >> x);
        assert(1 <= x && x <= N);
        assert(!A.is_final(x - 1));
        A.set_final(x - 1);
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < SIGMA; ++j) {
            int x; assert(cin >> x);
            assert(1 <= x && x <= N);
            A.set_edge(i, j, x - 1);
        }
    }

    cout << (A.tolerates() ? "DA" : "NU") << "\n";
}

int main() {
    ifstream cin("density.in");
    ofstream cout("density.out");

    int T; cin >> T;
    while (T--)
        test(cin, cout);
}
