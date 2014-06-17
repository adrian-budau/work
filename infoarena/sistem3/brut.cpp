#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Graph {
  public:
    Graph(int size, int modulo):
            edges_(size),
            gradient_(size),
            modulo_(modulo),
            inverse_(modulo) {
        for (int i = 0; i < modulo; ++i)
            inverse_[i] = pow(i, modulo - 2);
    }

    int size() const {
        return edges_.size();
    }

    void addEdge(int first_node, int second_node, int weight) {
        edges_[second_node].emplace_back(first_node, weight);
        edges_[first_node].emplace_back(second_node, weight);
    }

    void setGradient(int node, int gradient) {
        gradient_[node] = gradient;
    }

    void getPotentials() {
        potential_ = vector<int>(size(), -1);
        dfsBalanceGradients(0);

        potential_[0] = 0;
        dfsGetPotentials(0);
    }

    int potential(int node) const {
        return potential_[node];
    }

  private:
    int pow(int a, int b) {
        if (b == 0)
            return 1;
        if (b % 2)
            return pow(a, b - 1) * a % modulo_;
        int result = pow(a, b / 2);
        return result * result % modulo_;
    }

    void dfsBalanceGradients(int node, int from = -1) {
        for (auto &next : edges_[node])
            if (next.first != from)
                dfsBalanceGradients(next.first, node);
        if (from == -1)
            return;

        gradient_[from] = (gradient_[from] + gradient_[node]) % modulo_;
    }

    void dfsGetPotentials(int node, int from = -1) {
        if (from != -1) {
            int weight = 0;
            for (auto &next : edges_[node])
                if (next.first == from)
                    weight = next.second;
            potential_[node] = (potential_[from] - gradient_[node] * inverse_[weight]) % modulo_;
            if (potential_[node] < 0)
                potential_[node] += modulo_;
        }

        for (auto &next : edges_[node])
            if (next.first != from)
                dfsGetPotentials(next.first, node);
    }

    vector< vector< pair<int, int> > > edges_;
    vector<int> gradient_;
    int modulo_;

    vector<int> potential_;

    vector<int> inverse_;
};


int main() {
    ifstream cin("sistem3.in");
    ofstream cout("sistem3.out");

    int N, MODULO; cin >> N >> MODULO;

    Graph G(N, MODULO);

    for (int i = 0; i < N; ++i) {
        int x, y, w; cin >> x >> y >> w;
        if (x == y)
            continue;
        G.addEdge(x - 1, y - 1, w);
    }

    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        G.setGradient(i, x);
    }

    G.getPotentials();

    for (int i = 0; i < N; ++i)
        cout << G.potential(i) << "\n";
}
