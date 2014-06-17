#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class ModularArithmetics {
  public:
    ModularArithmetics(int modulo):
            modulo_(modulo),
            inverse_(modulo) {
        for (int i = 0; i < modulo; ++i)
            inverse_[i] = pow(i, modulo - 2);
    }

    int inverse(int value) const {
        return inverse_[value];
    }

    int pow(int value, int exponent) const {
        if (exponent == 0)
            return 1;
        if (exponent % 2)
            return pow(value, exponent - 1) * value % modulo_;
        int result = pow(value, exponent / 2);
        return result * result % modulo_;
    }

    int minus(int first, int second) const {
        first -= second;
        if (first < 0)
            first += modulo_;
        return first;
    }

    int plus(int first, int second) const {
        first += second;
        if (first >= modulo_)
            first -= modulo_;
        return first;
    }

    int divide(int first, int second) const {
        return first * inverse(second) % modulo_;
    }

    int multiply(int first, int second) const {
        return first * second % modulo_;
    }

  private:
    int modulo_;
    vector<int> inverse_;
};

class Graph {
  public:
    Graph(int size, int modulo):
            edges_(size),
            gradient_(size),
            arithmetics_(modulo) {
    }

    int size() const {
        return edges_.size();
    }

    int cycleSize() const {
        return cycle_.size();
    }

    void addEdge(int first_node, int second_node, int weight) {
        edges_[second_node].emplace_back(first_node, weight);
        if (first_node != second_node)
            edges_[first_node].emplace_back(second_node, weight);
    }

    void setGradient(int node, int gradient) {
        gradient_[node] = gradient;
    }

    void getPotentials() {
        potential_ = vector<int>(size(), -1);
        in_stack_ = in_cycle_ = vector<bool>(size(), false);
        dfsFindCycle(0);

        // remove cycle edges, we now have them separately
        separateCycle();

        for (int i = 0; i < cycleSize(); ++i)
            dfsBalanceGradients(cycle_[i]);

        solveCycle();

        for (int i = 0; i < cycleSize(); ++i)
            dfsGetPotentials(cycle_[i]);
    }

    int potential(int node) const {
        return potential_[node];
    }

  private:
    void dfsFindCycle(int node, int from = -1) {
        if (not cycle_.empty())
            return;
        in_stack_[node] = true;

        bool parent_seen = false;
        for (auto &next : edges_[node])
            if (next.first == from and not parent_seen)
                parent_seen = true;
            else if (in_stack_[next.first]) { // yey we found cycle
                cycle_.push_back(next.first);
                cycle_weight_.push_back(next.second);
                in_cycle_[cycle_.back()] = true;
                for (auto it = stack_.rbegin(); next.first != node and it != stack_.rend(); ++it) {
                    if (it -> first == next.first)
                        break;
                    cycle_.push_back(it -> first);
                    cycle_weight_.push_back(it -> second);
                    in_cycle_[cycle_.back()] = true;
                }
            } else {
                stack_.push_back(next);
                dfsFindCycle(next.first, node);
                stack_.pop_back();
            }

        in_stack_[node] = false;
    }

    void dfsBalanceGradients(int node, int from = -1) {
        for (auto &next : edges_[node])
            if (next.first != from)
                dfsBalanceGradients(next.first, node);
        if (from == -1)
            return;

        gradient_[from] = arithmetics_.plus(gradient_[from], gradient_[node]);
    }

    void dfsGetPotentials(int node, int from = -1) {
        if (from != -1) { // (potential_[from] - potential_[node]) * weight = gradient_[node]
            int weight = 0;
            for (auto &next : edges_[node])
                if (next.first == from)
                    weight = next.second;
            // potential_[node] = potential_[from] - (gradient_[node] / weight)
            potential_[node] =
                arithmetics_.minus(
                    potential_[from],
                    arithmetics_.divide(
                        gradient_[node],
                        weight));
        }

        for (auto &next : edges_[node])
            if (next.first != from)
                dfsGetPotentials(next.first, node);
    }

    void separateCycle() {
        for (int i = 0; i < size(); ++i)
            if (in_cycle_[i])
                for (int j = 0; j < int(edges_[i].size()); ++j) {
                    int next = edges_[i][j].first;
                    if (in_cycle_[next]) {
                        swap(edges_[i][j], edges_[i].back());
                        --j;
                        edges_[i].pop_back();
                    }
                }
    }

    void solveCycle() {
        // now let's sovle cycle
        // every element on the cycle tells us something like this
        // (cycle[i - 1] - cycle[i]) * weight[i - 1] +
        // (cycle[i + 1] - cycle[i]) * weight[i] = F[i]

        // if we say A[i] = (cycle[i] - cycle[i + 1])
        // and B[i] = weight[i]
        // and C[i] = gradient[i + 1]
        // then A[i] * B[i] - A[i + 1] * B[i + 1] = C[i]
        // and also A[0] + A[1] + A[2] + ... + A[N] = 0

        int N = cycleSize();
        vector<int> B(N), C(N);
        for (int i = 0; i < N; ++i) {
            B[i] = cycle_weight_[i];
            C[i] = gradient_[cycle_[(i + 1) % N]];
        }

        // if we add relations we get
        // A[0] * B[0] - A[i] * B[i] = C[0] + C[1] + ... + C[i - 1]
        // A[i] = A[0] * B[0] / B[i] - (C[0] + C[1] + C[2] + .. + C[i - 1]) / B[i]
        //
        // A[0] + A[1] + .. + A[N - 1] = 0 is now equivalent to
        // A[0] * (B[0] / B[0] + B[0] / B[1] + .. + B[0] / B[N - 1])
        //      = 0 / B[0] + C[0] / B[1] + (C[0] + C[1]) / B[2] + ...
        //        + (C[0] + C[1] + ... + C[N - 1]) / B[N]

        int coefficient = 1;
        int sum = 0;
        int partial_sum = 0;
        for (int i = 1; i < N; ++i) {
            coefficient =
                arithmetics_.plus(
                    coefficient,
                    arithmetics_.divide(B[0], B[i]));
            partial_sum = arithmetics_.plus(partial_sum, C[i - 1]);
            sum =
                arithmetics_.plus(
                        sum,
                        arithmetics_.divide(partial_sum, B[i]));
        }

        // now we have A[0], the rest are easy to obtain
        vector<int> A(N);
        A[0] = arithmetics_.divide(sum, coefficient); // we could be dividing by 0 but that's okay
        for (int i = 1; i < N; ++i)
            // A[i] = (A[i - 1] * B[i - 1] - C[i - 1]) / B[i]
            A[i] =
                arithmetics_.divide(
                    arithmetics_.minus(
                        arithmetics_.multiply(A[i - 1], B[i - 1]),
                        C[i - 1]),
                    B[i]);

        // now we can reconstruct the potentials
        potential_[cycle_.front()] = 0;
        for (int i = 1; i < N; ++i)
            potential_[cycle_[i]] =
                arithmetics_.minus(
                    potential_[cycle_[i - 1]],
                    A[i - 1]);
    }

    vector< vector< pair<int, int> > > edges_;
    vector<int> gradient_;

    vector<pair<int, int> > stack_; // we also keep the next edge weight
                                    // besides the node
    vector<bool> in_stack_;

    vector<int> cycle_, cycle_weight_;
    vector<bool> in_cycle_;
    vector<int> potential_;

    const ModularArithmetics arithmetics_;
};

int main() {
    ifstream cin("sistem3.in");
    ofstream cout("sistem3.out");

    int N, modulo; cin >> N >> modulo;

    Graph G(N, modulo);
    for (int i = 0; i < N; ++i) {
        int x, y, w; cin >> x >> y >> w;
        --x; --y;
        G.addEdge(x, y, w);
    }

    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        G.setGradient(i, x);
    }

    G.getPotentials();

    for (int i = 0; i < N; ++i)
        cout << G.potential(i) << "\n";
}
