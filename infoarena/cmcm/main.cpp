#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <stdexcept>

using namespace std;

const int kInfinite = numeric_limits<int>::max();

class BipartiteGraph {
  public:
    BipartiteGraph(int size_l, int size_r):
            size_l_(size_l),
            size_r_(size_r),
            edges_(size_l),
            leftMatch(size_l, -1),
            rightMatch(size_r, -1),
            costLeft(size_l, 0),
            costRight(size_r, 0) {
    }

    void addEdge(int from, int to, int distance) {
        edges_[from].push_back({to, distance});
    }

    int nodeMatch(int node) {
        return leftMatch[node];
    }

    pair<int, int> minimumWeightMatching() {
        int flowSize = 0;

        while (true) {
            vector<bool> usedLeft(size_l_, false), usedRight(size_r_, false);
            vector<int> tillEdge(size_r_, kInfinite);
            queue<int> Q;
            for (int i = 0; i < size_l_; ++i)
                if (leftMatch[i] == -1) {
                    usedLeft[i] = true;
                    Q.push(i);
                }

            auto updateRight = [&](int node) {
                usedRight[node] = true;

                // go left if possible
                if (rightMatch[node] != -1) {
                    tillEdge[node] = kInfinite;
                    usedLeft[rightMatch[node]] = true;
                    Q.push(rightMatch[node]);
                }
            };

            do {
                while (!Q.empty()) {
                    int node = Q.front();
                    Q.pop();

                    // pass the graph
                    for (auto &next : edges_[node]) {
                        if (not usedRight[next.to])
                             if (costLeft[node] + costRight[next.to] == next.distance
                                 && next.to != leftMatch[node])
                                    updateRight(next.to);
                        if (not usedRight[next.to] or rightMatch[next.to] == -1)
                            tillEdge[next.to] = min(tillEdge[next.to], next.distance - costLeft[node] - costRight[next.to]);
                    }
                }

                int nextChange = min_element(tillEdge.begin(), tillEdge.end()) - tillEdge.begin();
                int change = tillEdge[nextChange];
                if (change == kInfinite)
                    break;
                // we're still stuck here, let's extend
                for (int i = 0; i < size_l_; ++i)
                    if (usedLeft[i])
                        costLeft[i] += change;

                for (int i = 0; i < size_r_; ++i)
                    if (usedRight[i])
                        costRight[i] -= change;
                    else if (tillEdge[i] != kInfinite)
                        tillEdge[i] -= change;

                if (rightMatch[nextChange] == -1)
                    break;
                updateRight(nextChange);
            } while (true);

            // yey we got a new edge, maybe?
            bool didWe = false;
            used = vector<bool>(size_l_, false);
            for (int i = 0; i < size_l_; ++i)
                if (leftMatch[i] == -1)
                    if (match(i)) {
                        ++flowSize;
                        didWe = true;
                    }
            if (not didWe)
                break;
        }

        // and lucky for us the sum of y's is the answer
        int answer = 0;
        for (int i = 0; i < size_l_; ++i)
            if (leftMatch[i] != -1)
                answer += costLeft[i];
        for (int i = 0; i < size_r_; ++i)
            if (rightMatch[i] != -1)
                answer += costRight[i];
        return make_pair(flowSize, answer);
    }

  private:
    class Edge {
      public:
        Edge(int _to, int _distance):
                to(_to), distance(_distance) {}
        int to;
        int distance;
    };

    // hopcroft-karp style
    bool match(int node) {
        if (used[node])
            return false;
        used[node] = true;
        for (auto &next : edges_[node])
            if (costLeft[node] + costRight[next.to] == next.distance) {
                if (rightMatch[next.to] == -1) {
                    rightMatch[next.to] = node;
                    leftMatch[node] = next.to;
                    return true;
                }

                if (match(rightMatch[next.to])) {
                    rightMatch[next.to] = node;
                    leftMatch[node] = next.to;
                    return true;
                }
            }
        return false;
    }

    int size_l_, size_r_;
    vector< vector<Edge> > edges_;

    vector<int> leftMatch, rightMatch;
    vector<int> costLeft, costRight;

    vector<bool> used;
};

int main() {
    ifstream cin("cmcm.in");
    ofstream cout("cmcm.out");
    int N, M, E; cin >> N >> M >> E;

    vector< vector<int> > index(N, vector<int>(M, 0)),
                          cost(N, vector<int>(M, 0));

    BipartiteGraph G(N, M);

    int minEdge = 0;

    for (int i = 1; i <= E; ++i) {
        int from, to, value; cin >> from >> to >> value;
        --from; --to;

        cost[from][to] = value;
        index[from][to] = i;
        minEdge = min(minEdge, value);
    }

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (index[i][j])
                G.addEdge(i, j, cost[i][j] - minEdge);

    int flow, answer;
    tie(flow, answer) = G.minimumWeightMatching();
    answer += minEdge * flow;

    cout << flow << " " << answer << "\n";
    for (int i = 0; i < N; ++i)
        if (G.nodeMatch(i) != -1)
            cout << index[i][G.nodeMatch(i)] << " ";
    cout << "\n";
}
