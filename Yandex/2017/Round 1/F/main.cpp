#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Tree {
  public:
    Tree(int size):
        m_edges(size) {}

    void add_edge(int x, int y) {
        m_edges[x].push_back(y);
        m_edges[y].push_back(x);
    }

    int min_cover() const {
        return dfs(0).second + 1;
    }

  private:
    pair<int, int> dfs(int node, int father = -1) const {
        int mask = 0;
        int answer = 0;
        int now = 0;
        for (auto &son : m_edges[node])
            if (son != father) {
                auto aux = dfs(son, node);
                answer = max(answer, aux.second);
                for (int i = answer; i >= 0; --i)
                    if (((1 << i) & mask) && ((1 << i) & aux.first)) {
                        now = max(now, i + 1);
                        break;
                    }
                mask |= aux.first;
            }
        while ((1 << now) & mask)
            ++now;
        for (int i = 0; i < now; ++i)
            if ((1 << i) & mask)
                mask ^= (1 << i);
        answer = max(answer, now);
        mask |= (1 << now);
        return make_pair(mask, answer);
    }

    vector< vector<int> > m_edges;
};

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;
    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        T.add_edge(x - 1, y - 1);
    }

    cout << T.min_cover() << "\n";
}
