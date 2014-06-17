#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int partitions[14] = {1, 1, 2, 3, 5, 7, 11, 15, 22, 30, 42, 56, 77, 101};

class Graph {
  public:
    Graph(int size):
        edges_(size),
        disconnected_(size, 0) {
    }

    void addEdge(int x, int y) {
        edges_[x].emplace_back(y);
        edges_[y].emplace_back(x);
    }

    void prepare() {
        dfs(0);
    }

    void disconnect(int node) {
        disconnected_[node] = 1;
    }

    void connect(int node) {
        disconnected_[node] = 0;
    }

    vector<int> partitions() const {
        vector<int> answer(edges_.size() + 1, 0);
        for (int i = 0; i < int(edges_.size()); ++i)
            if (disconnected_[i])
                answer[dfs(i)]++;
        return answer;
    }

  private:
    int dfs(int node) const {
        int answer = 1;
        for (auto &next : edges_[node])
            if (not disconnected_[next])
                answer += dfs(next);
        return answer;
    }

    void dfs(int node, int from = -1) {
        for (int i = 0; i < int(edges_[node].size()); ++i)
            if (edges_[node][i] == from) {
                swap(edges_[node][i], edges_[node].back());
                edges_[node].pop_back();
                --i;
            } else
                dfs(edges_[node][i], node);
    }

    vector< vector<int> > edges_;
    vector<int> disconnected_;
};

int main() {
    ifstream cin("avd.in");
    ofstream cout("avd.out");

    int T; cin >> T;

    for (int i = 0; i < T; ++i) {
        int N; cin >> N;

        Graph G(N);
        for (int i = 1; i < N; ++i) {
            int x; int y; cin >> x >> y;
            G.addEdge(x - 1, y - 1);
        }

        G.prepare();

        G.disconnect(0);
        set< vector<int> > S;

        for (int i = 0; i < (1 << (N - 1)); ++i) {
            for (int j = 0; j < N - 1; ++j)
                if ((1 << j) & i)
                    G.connect(j + 1);
                else
                    G.disconnect(j + 1);
            S.insert(G.partitions());
        }

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(5);
        cout << double(S.size()) / double(partitions[N]) << "\n";
    }
}
