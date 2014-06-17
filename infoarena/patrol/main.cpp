#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>
#include <limits>
#include <queue>

using namespace std;

class CostGraph {
  public:
    CostGraph(int size):
            size_(size),
            edges_(size),
            cost_(size) {
    }

    void addEdge(int from, int to) {
        edges_[from].push_back(to);
    }

    void setCost(int node, int cost) {
        cost_[node] = cost;
    }

    int minCost(int from, const vector<int> &destinations) {
        queue<int> Q;
        vector<bool> inQueue(size_);
        vector<int> distance(size_, numeric_limits<int>::max());

        Q.push(from);
        inQueue[from] = true;
        distance[from] = cost_[from];

        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            inQueue[node] = false;

            for (auto &next : edges_[node])
                if (distance[next] > distance[node] + cost_[next]) {
                    distance[next] = distance[node] + cost_[next];
                    if (not inQueue[next]) {
                        inQueue[next] = true;
                        Q.push(next);
                    }
                }
        }

        int answer = numeric_limits<int>::max();
        for (auto &node : destinations)
            answer = min(answer, distance[node]);
        return answer;
    }

  protected:
    int size_;
    vector< vector<int> > edges_;
    vector<int> cost_;
};

class CopGraph final : public CostGraph {
  public:
    CopGraph(int size):
            CostGraph(size) {
    }

    void addCop(vector<int> cop) {
        int N = cop.size();
        for (int i = N - 2; i > 0; --i)
            cop.push_back(cop[i]);

        cops_.push_back(cop);
        period_ = period_ * (cop.size()) / cmmdc(period_, cop.size());
    }

    int minCostRun(int from, int to) const {
        CostGraph G(period_ * size_);
        for (int i = 0; i < period_; ++i)
            for (int j = 0; j < size_; ++j)
                G.setCost(i * size_ + j, cost_[j]);

        for (int i = 0; i < period_; ++i) {
            set<pair<int, int> > bad_edges;
            vector<bool> bad_node(size_, false);

            for (auto &cop : cops_) {
                int now_from = cop[i % cop.size()];
                int now_to = cop[(i + 1) % cop.size()];

                bad_edges.insert({now_from, now_to});
                bad_edges.insert({now_to, now_from});
                bad_node[now_to] = true;
            }

            for (int j = 0; j < size_; ++j)
                for (auto &next : edges_[j]) {
                    if (bad_edges.count({j, next}))
                        continue;
                    if (bad_node[next])
                        continue;
                    G.addEdge(i * size_ + j, (i + 1) % period_ * size_ + next);
                }
        }

        vector<int> destinations(period_);
        for (int i = 0; i < period_; ++i)
            destinations[i] = i * size_ + to;
        return G.minCost(from, destinations);
    }

  private:
    int cmmdc(int a, int b) const {
        if (b == 0)
            return a;
        return cmmdc(b, a % b);
    }

    vector< vector<int> > cops_;
    int period_ = 1;
};

int main() {
    ifstream cin("patrol.in");
    ofstream cout("patrol.out");

    int N, M, P; cin >> N >> M >> P;

    CopGraph G(N);
    for (int i = 0; i < N; ++i) {
        int cost; cin >> cost;
        G.setCost(i, cost);
    }

    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        G.addEdge(x - 1, y - 1);
        G.addEdge(y - 1, x - 1);
    }

    for (int i = 0; i < P; ++i) {
        int L; cin >> L;
        vector<int> cop(L);
        for (int j = 0; j < L; ++j) {
            cin >> cop[j];
            --cop[j];
        }
        G.addCop(cop);
    }

    cout << G.minCostRun(0, N - 1) << "\n";
}
