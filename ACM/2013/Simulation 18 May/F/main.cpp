#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cstring>
#include <limits>
#include <algorithm>

using namespace std;

const int kMaxN = 105;

int S, D;
vector<int> E[kMaxN * 5], B[kMaxN];

int C[kMaxN * 5][kMaxN * 5];
int A[kMaxN * 5];
int used[kMaxN], l[kMaxN], r[kMaxN];

class Graph {
  public:
    explicit Graph(const int &size):
            size_(size),
            edges_(size),
            capacity_(size, vector<int>(size, 0)) {
    }

    void addEdge(const int &from, const int &to, int flow) {
        edges_[from].push_back(to);
        edges_[to].push_back(from);
        capacity_[from][to] = flow;
    }

    int maxFlow(const int &source, const int &sink) {
        int max_flow = 0;

        levelGraph(sink);
        preferred_ = vector< vector<int>::iterator >(size_);
        for (int i = 0;  i < size_; ++i)
            preferred_[i] = edges_[i].begin();
        from_ = vector<int>(size_);

        int current_node = source;
        while (level_[source] < size_) {
            vector<int>::iterator it;
            for (it = preferred_[current_node]; it != edges_[current_node].end(); ++it)
                if (level_[current_node] == level_[*it] + 1 and capacity_[current_node][*it] > 0) {
                    break;
                }

            if (it == edges_[current_node].end()) {
                preferred_[current_node] = edges_[current_node].begin();

                if (not retreat(current_node))
                    break;
                if (current_node != source)
                    current_node = from_[current_node];
                continue;
            }

            preferred_[current_node] = it;
            from_[*it] = current_node;
            current_node = *it;

            if (current_node == sink) {
                max_flow += augment(source, sink);
                current_node = source;
            }
        }

        return max_flow;
    }

    int cap(int x, int y) {
        return capacity_[x][y];
    }

  private:
    int retreat(const int &node) {
        int old_distance = level_[node];
        level_[node] = numeric_limits<int>::max();
        for (vector<int>::iterator it = edges_[node].begin(); it != edges_[node].end(); ++it)
            if (capacity_[node][*it] > 0)
                if (level_[*it] != numeric_limits<int>::max() && level_[*it] + 1 < level_[node])
                    level_[node] = level_[*it] + 1;
        if (level_[node] == numeric_limits<int>::max())
            return 0;

        ++many_[level_[node]];
        return (--many_[old_distance]);
    }

    int augment(const int &source, const int &sink) {
        int flow = numeric_limits<int>::max();
        for (int node = sink; node != source; node = from_[node]) {
            int tmp = capacity_[from_[node]][node];
            if (tmp < flow)
                flow = tmp;
        }

        if (not flow)
            return 0;

        for (int node = sink; node != source; node = from_[node]) {
            capacity_[from_[node]][node] -= flow;
            capacity_[node][from_[node]] += flow;
        }

        return flow;
    }

    void levelGraph(const int &source) {
        queue<int> Q;
        level_ = vector<int>(size_, std::numeric_limits<int>::max());
        many_ = vector<int>(size_, 0);

        Q.push(source);
        level_[source] = 0;

        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();

            ++many_[level_[node]];
            for (vector<int>::iterator it = edges_[node].begin(); it != edges_[node].end(); ++it)
                if (level_[*it] == std::numeric_limits<int>::max() && capacity_[*it][node] > 0) {
                    level_[*it] = level_[node] + 1;
                    Q.push(*it);
                }
        }
    }


    int size_;
    vector< vector<int> > edges_;
    vector< vector<int> > capacity_;

    vector<int> level_, many_, from_;
    vector< vector<int>::iterator > preferred_;
};

bool pairup(int nod) {
    if (used[nod])
        return false;
    used[nod] = 1;
    for (vector<int>::iterator it = B[nod].begin(); it != B[nod].end(); ++it)
        if (!r[*it]) {
            l[nod] = *it;
            r[*it] = nod;
            return true;
        }

    for (vector<int>::iterator it = B[nod].begin(); it != B[nod].end(); ++it)
        if (pairup(r[*it])) {
            l[nod] = *it;
            r[*it] = nod;
            return true;
        }
    return false;
}

int main() {
    ifstream cin("factory.in");
    ofstream cout("factory.out");

    int N, K; cin >> N >> K;

    S = 0;
    D = 2 * N + 1;
    Graph G(2 * N + 2);
    for (int i = 0; i < N; ++i) {
        string T; cin >> T;
        for (int j = 0; j < N; ++j)
            if (T[j] == '+') {
                G.addEdge(i + 1, j + N + 1, 1);
            }
    }

    for (int i = 1; i <= N; ++i) {
        G.addEdge(S, i, K);
        G.addEdge(i + N, D, K);
    }

    if (G.maxFlow(S, D) != K * N) {
        cout << "-1\n";
        return 0;
    }


    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= N; ++j)
            if (G.cap(j + N, i) == 1)
                B[i].push_back(j);

    bool ok;
    for (int _ = 0; _ < K; _++) {
        memset(l, 0, sizeof(l));
        memset(r, 0, sizeof(r));
        do {
            ok = false;
            memset(used, 0, sizeof(used));
            for (int i = 1; i <= N; ++i)
                if (!l[i])
                    ok |= pairup(i);
        } while (ok);

        for (int i = 1; i <= N; ++i) {
            cout << l[i] << " ";
        }
        cout << "\n";

        for (int i = 1; i <= N; ++i)
            B[i].erase(find(B[i].begin(), B[i].end(), l[i]));
    }
}
