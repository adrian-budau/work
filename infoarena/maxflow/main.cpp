#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

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

int main() {
    ifstream cin("maxflow.in");
    ofstream cout("maxflow.out");

    int N, M; cin >> N >> M;

    Graph G(N);
    for (int i = 0; i < M; ++i) {
        int x, y, z; cin >> x >> y >> z;
        G.addEdge(x - 1, y - 1, z);
    }

    cout << G.maxFlow(0, N - 1) << "\n";
}

