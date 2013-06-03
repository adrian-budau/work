#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <queue>

using namespace std;

class Graph {
  public:
    Graph(const int &size):
        size_(size + 2),
        source_(size),
        sink_(size + 1),
        edges_(size_),
        capacity_(size_, vector<int>(size_, 0)),
        lower_bound_(size_, vector<int>(size_, 0)),
        lower_bound_sum_(size_, 0) {
    }

    void addEdge(int from, int to, int lower_bound, int capacity) {
        edges_[from].push_back(to);
        edges_[to].push_back(from);

        capacity_[from][to] += capacity - lower_bound;
        lower_bound_[from][to] += lower_bound;

        lower_bound_sum_[from] += lower_bound;
        lower_bound_sum_[to] -= lower_bound;
    }

    int maxFlow() {
        int expected_source = 0, expected_sink = 0;
        for (int i = 0; i < source_; ++i)
            if (lower_bound_sum_[i] > 0) {
                capacity_[i][sink_] = lower_bound_sum_[i];
                edges_[i].push_back(sink_);
                edges_[sink_].push_back(i);
                expected_sink += lower_bound_sum_[i];
            } else if (lower_bound_sum_[i] < 0) {
                capacity_[source_][i] = -lower_bound_sum_[i];
                edges_[source_].push_back(i);
                expected_source -= lower_bound_sum_[i];
            }

        int flow = 0;
        while (bfs(source_, sink_))
            for (auto &from : edges_[sink_]) {
                if (from_[from] == -1)
                    continue;

                from_[sink_] = from;
                int current_flow = numeric_limits<int>::max();
                for (int i = sink_; i != source_; i = from_[i])
                    current_flow = min(current_flow, capacity_[from_[i]][i]);
                if (not current_flow)
                    continue;
                for (int i = sink_; i != source_; i = from_[i]) {
                    capacity_[from_[i]][i] -= current_flow;
                    capacity_[i][from_[i]] += current_flow;
                }

                flow += current_flow;
            }
        if (flow != expected_source)
            return -1;
        return flow;
    }

    int flow(int from, int to) {
        return lower_bound_[from][to] + capacity_[to][from];
    }

  private:
    bool bfs(int source, int sink) {
        queue<int> Q;
        Q.push(source);
        from_ = vector<int>(size_, -1);
        from_[source] = source;

        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            if (node == sink)
                continue;
            for (auto &neighbour : edges_[node])
                if (from_[neighbour] == -1 and capacity_[node][neighbour] > 0) {
                    from_[neighbour] = node;
                    Q.push(neighbour);
                }
        }

        return from_[sink] != -1;
    }

    int size_, source_, sink_;
    vector< vector<int> > edges_;
    vector< vector<int> > capacity_;
    vector< vector<int> > lower_bound_;

    vector<int> lower_bound_sum_;

    vector<int> from_;
};

int main() {
    ifstream cin("cooling.in");
    ofstream cout("cooling.out");
    
    int N, M; cin >> N >> M;

    Graph G(N);

    vector< pair<int, int> > E(M);
    for (int i = 0; i < M; ++i) {
        int from, to, lower_bound, capacity; cin >> from >> to >> lower_bound >> capacity;
        --from; --to;
        G.addEdge(from, to, lower_bound, capacity);
        E[i] = {from, to};
    }

    if (G.maxFlow() == -1) {
        cout << "NO\n";
        return 0;
    }

    cout << "YES\n";
    for (int i = 0; i < M; ++i)
        cout << G.flow(E[i].first, E[i].second) << "\n";
}
