#include <iostream>
#include <vector>
#include <fstream>
#include <list>

using namespace std;

class Graph {
  public:
    Graph(int size, int caterpillars):
            size_(size),
            caterpillars_(caterpillars),
            edges_(size),
            worker_(size, {-1, 0}),
            position_(caterpillars, -1) {
    }

    void addEdge(int from, int to) {
        edges_[from].push_back(to);
    }

    void addCaterpillar(int node, int index, char side) {
        position_[index] = node;
        worker_[node] = {index, side};
    }

    void promoteCaterpillars() {
        left_.resize(size_);
        right_.resize(size_);
        dfsPromote(0);
    }

    int node(int caterpillar) const {
        return position_[caterpillar];
    }

  private:
    void join(list<int> &A, list<int> &B) {
        A.splice(A.end(), B);
    }

    void dfsPromote(int node) {

        for (auto &next : edges_[node]) {
            dfsPromote(next);
            join(left_[node], left_[next]);
            join(right_[node], right_[next]);
        }

        left_[node].push_back(node);
        right_[node].push_front(node);

        if (worker_[node].second == 'S') {
            while (left_[node].size()) {
                int next = left_[node].front();
                left_[node].pop_front();
                if (worker_[next].second == 0) { // yey we get promoted
                    swap(worker_[next], worker_[node]);
                    position_[worker_[next].first] = next;
                    break;
                }
            }
        } else if (worker_[node].second == 'D') {
            while (right_[node].size()) {
                int next = right_[node].back();
                right_[node].pop_back();
                if (worker_[next].second == 0) { // yey we get promoted for right
                    swap(worker_[next], worker_[node]);
                    position_[worker_[next].first] = next;
                    break;
                }
            }
        }
    }

    int size_, caterpillars_;
    vector< vector<int> > edges_;
    vector< pair<int, char> > worker_;
    vector< int> position_;

    vector< list<int> > left_, right_;
};

int main() {
    ifstream cin("omizi.in");
    ofstream cout("omizi.out");

    int N, M; cin >> N >> M;
    Graph G(N, M);

    for (int i = 0; i < N; ++i) {
        while (true) {
            int node; cin >> node;
            if (not node)
                break;
            G.addEdge(i, node - 1);
        }
    }

    for (int i = 0; i < M; ++i) {
        int node; char side;
        cin >> node >> side;
        G.addCaterpillar(node - 1, i, side);
    }

    G.promoteCaterpillars();

    for (int i = 0; i < M; ++i)
        cout << G.node(i) + 1 << "\n";
}
