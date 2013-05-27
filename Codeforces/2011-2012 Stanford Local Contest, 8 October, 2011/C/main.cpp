#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

const int kLog = 17;

class Graph {
  public:
    Graph(const int &size):
        size_(size),
        edges_(size) {
    }

    void addEdge(int from, int to, int cost) {
        for (int i = 0; i < 2; ++i) {
            edges_[from].push_back({to, cost});
            swap(from, to);
        }
    }

    void prepare() {
        used_ = vector<int>(size_, 0);
        dfsFindCycle(0);

        cycle_size_ = cycle_.size();

        depth_ = vector<int>(size_, numeric_limits<int>::max());
        distance_ = vector<int>(size_, numeric_limits<int>::max());
        ancestors_.resize(size_);
        order_ = vector<int>(size_, -1);

        for (auto &x : cycle_) {
            distance_[x.first] = 0;
            depth_[x.first] = 0;
            ancestors_[x.first] = vector<int>(kLog, x.first);
        }

        for (int i = 0; i < cycle_size_; ++i) {
            int node = cycle_[i].first;
            order_[node] = i;
            dfsAncestors(node);
        }

        sums_ = vector<int>(cycle_size_, 0);
        for (int i = 0; i < cycle_size_; ++i) {
            sums_[i] = cycle_[i].second;
            if (i > 0)
                sums_[i] += sums_[i - 1];
        }
    }

    int query(int from, int to) {
        if (order_[from] != order_[to]) {
            int answer = distance_[from] + distance_[to];
            from = order_[from];
            to = order_[to];
            if (from > to)
                swap(from, to);

            int aux = sums_[to - 1] - (from ? sums_[from - 1] : 0);
            aux = min(aux, sums_.back() - aux);
            return answer + aux;
        }

        int z = lca(from, to);
        return distance_[from] + distance_[to] - 2 * distance_[z];
    }

  private:
    void dfsFindCycle(int node) {
        used_[node] = 1;
        for (auto &x : edges_[node])
            if (used_[x.first] and not cycle_.size() and stack_.back().first != x.first) {
                for (auto it = stack_.begin(); it != stack_.end(); ++it)
                    if (it -> first == x.first)
                        cycle_.insert(cycle_.begin(), it, stack_.end());
                cycle_.push_back({node, x.second});
            } else if (not used_[x.first]) {
                stack_.push_back({node, x.second});
                dfsFindCycle(x.first);
                stack_.pop_back();
            }
    }

    void dfsAncestors(int node) {
        for (int i = 1; i < kLog; ++i)
            ancestors_[node][i] = ancestors_[ancestors_[node][i - 1]][i - 1];

        for (auto &x : edges_[node])
            if (distance_[node] + x.second < distance_[x.first]) {
                depth_[x.first] = depth_[node] + 1;
                distance_[x.first] = distance_[node] + x.second;
                ancestors_[x.first] = vector<int>(kLog, node);
                order_[x.first] = order_[node];
                dfsAncestors(x.first);
            }
    }

    int lca(int x, int y) {
        if (depth_[x] < depth_[y])
            swap(x, y);
        for (int i = 0; i < kLog; ++i)
            if ((1 << i) & (depth_[x] - depth_[y]))
                x = ancestors_[x][i];
        if (x == y)
            return x;
        for (int i = kLog - 1; i >= 0; --i)
            if (ancestors_[x][i] != ancestors_[y][i]) {
                x = ancestors_[x][i];
                y = ancestors_[y][i];
            }
        return ancestors_[x][0];
    }

    int size_;

    vector< vector< pair<int, int> > > edges_;

    int cycle_size_;
    vector< pair<int, int> > cycle_;

    vector<int> used_;
    vector< pair<int, int> > stack_;

    vector<int> depth_;
    vector<int> distance_;
    vector< vector<int> > ancestors_;
    vector<int> order_;
    vector<int> sums_;
};

int main() {
    int N;
    while (cin >> N) {
        if (N == 0)
            break;
        Graph G(N);
        for (int i = 0; i < N; ++i) {
            int x, y, c; cin >> x >> y >> c;
            G.addEdge(x, y, c);
        }

        G.prepare();

        int Q; cin >> Q;
        for (int i = 0; i < Q; ++i) {
            int x, y; cin >> x >> y;
            cout << G.query(x, y) << "\n";
        }

    }
}
