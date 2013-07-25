#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Graph {
  public:
    Graph(int size):
            edges_(size),
            costs_(size) {
    }

    void addEdge(const int &from, const int &to) {
        edges_[from].push_back(to);
        edges_[to].push_back(from);
    }

    void setCost(int node, int value) {
        costs_[node] = value;
    }

    int cost() {
        used_ = vector<int>(edges_.size(), 0);

        for (int i = 0; i < int(edges_.size()); ++i)
            if (not used_[i])
                groups_.push_back(costComponent(i));

        if (groups_.size() == 1)
            return 0;
        nth_element(groups_.begin(), groups_.begin(), groups_.end());

        if (groups_[0] < 0)
            return -1;
        int answer = 0;
        for (int i = 1; i < int(groups_.size()); ++i)
            answer += groups_[0] + groups_[i];

        return answer;
    }

  private:
    int costComponent(int node) {
        used_[node] = 1;

        int answer = costs_[node];
        for (vector<int>::iterator it = edges_[node].begin(); it != edges_[node].end(); ++it)
            if (not used_[*it]) {
                int aux = costComponent(*it);
                if (answer < 0)
                    answer = aux;
                else if (aux >= 0 and aux < answer)
                    answer = aux;
            }
        return answer;
    }

    vector< vector<int> > edges_;
    vector<int> costs_;
    vector<int> used_;
    vector<int> groups_;
};

int main() {
    int N, M; cin >> N >> M;

    Graph G(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        G.addEdge(x - 1, y - 1);
    }

    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        G.setCost(i, x);
    }

    cout << G.cost() << "\n";
}
