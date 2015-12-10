#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

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
        m_depth = vector<int>(size(), 0);
        m_color = vector<int>(size(), WHITE);
        m_delta = vector<int>(size(), 0);
        dfs(0);

        assert(m_color == vector<int>(size(), BLACK));

        vector<int> answer;
        dfsSolve(0, answer);
        return answer;
    }

  private:
    enum {
        WHITE = 0,
        GRAY = 1,
        BLACK = 2
    };

    void dfs(int node) {
        m_color[node] = GRAY;
        m_stack.push_back(node);

        for (auto &other : m_long_edges[node])
            if (m_color[other] == GRAY && other != node) {
                m_delta[m_stack[m_depth[other] + 1]]--;
                m_delta[node]++;
            } else if (m_color[other] == BLACK) {
                m_delta[0]--;
                m_delta[node]++;
                m_delta[other]++;
            }

        for (auto &son : m_edges[node])
            if (m_color[son] == WHITE) {
                m_depth[son] = m_depth[node] + 1;
                dfs(son);
            }

        m_color[node] = BLACK;
        m_stack.pop_back();
    }

    void dfsSolve(int node, vector<int> &answer, int total = 0, int from = -1) {
        total += m_delta[node];

        if (total >= 0)
            answer.push_back(node);

        for (auto &son : m_edges[node])
            if (son != from)
                dfsSolve(son, answer, total, node);

        total -= m_delta[node];
    }

    vector< vector<int> > m_edges, m_long_edges;
    vector<int> m_depth;
    vector<int> m_color;
    vector<int> m_delta;
    vector<int> m_stack;
};

int main() {
    ifstream cin("treespotting.in");
    ofstream cout("treespotting.out");

    int N, M;
    assert(cin >> N >> M);
    assert(2 <= N && N <= 100000);
    assert(N - 1 <= M && M <= 150000);

    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int x, y;
        assert(cin >> x >> y);
        assert(1 <= x && x <= N);
        assert(1 <= y && y <= N);
        --x; --y;
        T.addEdge(x, y);
    }

    for (int i = N - 1; i < M; ++i) {
        int x, y;
        assert(cin >> x >> y);
        assert(1 <= x && x <= N);
        assert(1 <= y && y <= N);
        --x; --y;
        T.addLongEdge(x, y);
    }

    auto answer = T.possibleRoots();
    sort(answer.begin(), answer.end());
    assert(answer.size());

    cout << answer.size() << "\n";
    for (auto &x : answer)
        cout << x + 1 << " ";
    cout << "\n";
}
