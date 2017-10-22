#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class FenwickTree {
  public:
    FenwickTree(int size):
        m_data(size + 5, 0),
        m_size(size) {
    }

    void add(int x, int y) {
        ++x;
        for (; x <= m_size; x += (x & -x))
            m_data[x] += y;
    }

    int query(int x, int y) {
        return query(y) - query(x - 1);
    }

    int query(int x) {
        ++x;
        int total = 0;
        for (; x > 0; x-= (x & -x))
            total += m_data[x];
        return total;
    }

  private:
    vector<int> m_data;
    int m_size;
};

class Tree {
  public:
    Tree(int size):
            m_edges(size),
            m_value(size),
            m_big_value(size),
            m_begin(size),
            m_end(size),
            m_mark(size, false) {
    }

    void addEdge(int x, int y) {
        m_edges[x].push_back(y);
    }

    void setValue(int pos, int64_t x) {
        m_value[pos] = x;
    }

    void setBigValue(int pos, int64_t y) {
        m_big_value[pos] = y;
    }

    int size() const {
        return m_edges.size();
    }

    vector<int64_t> answer() {
        int time = 0;
        dfs(0, time);

        vector< pair<int64_t, int> > events;
        events.reserve(2 * size());
        for (int i = 0; i < size(); ++i) {
            events.push_back(make_pair(m_value[i], -i - 1));
            events.push_back(make_pair(m_big_value[i], i));
        }

        sort(events.begin(), events.end());
        FenwickTree T(size());
        vector<int64_t> result(size(), 0);
        for (auto &e : events) {
            if (e.second < 0) {
                int index = -(e.second + 1);
                T.add(m_begin[index], 1);
                m_mark[index] = true;
            } else {
                int node = e.second;
                int total = 0;
                for (auto &x : m_edges[node]) {
                    int aux = T.query(m_begin[x], m_end[x] - 1);
                    total += aux;
                    result[node] -= 1LL * aux * aux;
                }
                result[node] += 1LL * total * total;
                if (m_mark[node])
                    result[node] += total * 2 + 1;
            }
        }

        return result;
    }

  private:
    void dfs(int node, int& time) {
        m_begin[node] = time++;
        for (auto &x : m_edges[node])
            dfs(x, time);
        m_end[node] = time;
    }

    vector< vector<int> > m_edges;
    vector<int64_t> m_value, m_big_value;

    vector<int> m_begin, m_end;
    vector<bool> m_mark;
};

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int x; cin >> x;
        --x;
        T.addEdge(x, i);
    }

    for (int i = 0; i < N; ++i) {
        int64_t x; cin >> x;
        T.setValue(i, x);
    }

    for (int i = 0; i < N; ++i) {
        int64_t x; cin >> x;
        T.setBigValue(i, x);
    }

    for (auto x : T.answer())
        cout << x << "\n";
}
