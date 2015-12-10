#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Tree {
  public:
    Tree(int size):
            m_edges(size),
            m_long_edges(size) {
    }

    void addEdge(int x, int y) {
        m_edges[x].push_back(y);
        m_edges[y].push_back(x);
    }

    void addLongEdge(int x, int y) {
        m_long_edges[x].push_back(y);
        m_long_edges[y].push_back(x);
    }

    int size() const {
        return m_edges.size();
    }

    vector<int> possibleRoots() {
        vector<int> answer;
        for (int i = 0; i < size(); ++i) {
            m_color = vector<int>(size(), WHITE);
            if (dfs(i))
                answer.push_back(i);
        }
        return answer;
    }

  private:
    enum {
        WHITE = 0,
        GRAY = 1,
        BLACK = 2
    };

    bool dfs(int node) {
        m_color[node] = GRAY;

        for (auto &other : m_long_edges[node])
            if (m_color[other] == BLACK)
                return false;

        for (auto &son : m_edges[node])
            if (m_color[son] == WHITE)
                if (not dfs(son))
                    return false;

        m_color[node] = BLACK;
        return true;
    }

    vector< vector<int> > m_edges, m_long_edges;
    vector<int> m_color;
};

int main() {
    ifstream cin("treespotting.in");
    ofstream cout("treespotting.out");

    int N, M; cin >> N >> M;

    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y; --x; --y;
        T.addEdge(x, y);
    }

    for (int i = N - 1; i < M; ++i) {
        int x, y; cin >> x >> y; --x; --y;
        T.addLongEdge(x, y);
    }

    auto answer = T.possibleRoots();
    sort(answer.begin(), answer.end());

    cout << answer.size() << "\n";
    for (auto &x : answer)
        cout << x + 1 << " ";
    cout << "\n";
}
