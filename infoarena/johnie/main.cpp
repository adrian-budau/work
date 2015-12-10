#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
  public:
    Graph(int size):
            m_edge(size + 1) {
    }

    int size() const {
        return m_edge.size();
    }

    void addEdge(int x, int y) {
        m_edge[x].push_back(y);
        m_edge[y].push_back(x);
    }

    vector< vector<int> > eulerCover() {
        for (int i = 1; i < size(); ++i)
            if (m_edge[i].size() % 2) {
                m_edge[0].push_back(i);
                m_edge[i].push_back(0);
            }
        vector<int> chain;
        if (m_edge[0].empty()) {
            dfs(1, chain);
            return vector< vector<int> >(1, chain);
        } else {
            dfs(0, chain);

            vector< vector<int> > answer;
            vector<int> aux;
            for (auto &v : chain)
                if (v == 0) {
                    if (aux.size())
                        answer.push_back(aux);
                    aux.clear();
                } else
                    aux.push_back(v);
            return answer;
        }
    }

  private:
    void dfs(int node, vector<int> &chain) {
        while (!m_edge[node].empty()) {
            int next = m_edge[node].back();
            m_edge[node].pop_back();
            for (auto it = m_edge[next].begin(); it != m_edge[next].end(); ++it)
                if (*it == node) {
                    swap(*it, m_edge[next].back());
                    m_edge[next].pop_back();
                    break;
                }
            dfs(next, chain);
        }

        chain.push_back(node);
    }
    vector< vector<int> > m_edge;
};

int main() {
    ifstream cin("johnie.in");
    ofstream cout("johnie.out");

    int N, M; cin >> N >> M;
    Graph G(N);

    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        G.addEdge(x, y);
    }

    auto answer = G.eulerCover();
    cout << answer.size() << "\n";
    for (auto &aux : answer) {
        cout << aux.size() << " ";
        for (auto &v : aux)
            cout << v << " ";
        cout << "\n";
    }
}
