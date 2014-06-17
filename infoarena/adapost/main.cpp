#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <limits>
#include <stdexcept>

using namespace std;

typedef pair<double, double> Soldier, Shelter;

const double kInfinite = 1e50;

const double kEpsilon = 1e-6;

int cmp(double a, double b) {
    if (a + kEpsilon < b)
        return -1;
    if (b + kEpsilon < a)
        return 1;
    return 0;
}

bool equal(double a, double b) {
    return cmp(a, b) == 0;
}

class SimpleGraph {
  public:
    SimpleGraph(int size, int source, int sink):
            size_(size),
            source_(source),
            sink_(sink),
            flow_(0),
            edges_(size),
            parent_(size, -1) {
        queue_.push(source);
        parent_[source] = source;
    }

    void addEdge(int from, int to) {
        edges_[from].push_back(to);
        if (parent_[from] != -1 and parent_[to] == -1) {
            parent_[to] = from;
            queue_.push(to);
        }

        increaseChain();
    }

    const vector<int>& edges(int node) const {
        return edges_[node];
    }

    int flow() const {
        return flow_;
    }

  private:
    void increaseChain() {
        while (!queue_.empty()) {
            int node = queue_.front();
            queue_.pop();
            if (node == sink_)
                break;

            for (auto &next : edges_[node])
                if (parent_[next] == -1) {
                    parent_[next] = node;
                    queue_.push(next);
                }
        }

        if (parent_[sink_] == -1)
            return;
        ++flow_;
        for (int node = sink_; node != source_; node = parent_[node]) {
            vector<int> &where = edges_[parent_[node]];
            where.erase(find(where.begin(), where.end(), node));
            edges_[node].push_back(parent_[node]);
        }

        queue_ = queue<int>();
        parent_ = vector<int>(size_, -1);

        queue_.push(source_);
        parent_[source_] = source_;
    }

    int size_, source_, sink_, flow_;
    vector< vector<int> > edges_;
    queue<int> queue_;
    vector<int> parent_;
};

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

    void addEdge(int from, int to, double distance) {
        edges_[from].push_back({to, distance});
    }

    double minimumWeightMatching() {
        if (size_l_ != size_r_)
            throw new runtime_error("Graph is not regular");

        int steps = size_l_;

        while (steps) {
            vector<bool> usedLeft(size_l_, false), usedRight(size_r_, false);
            vector<double> tillEdge(size_r_, kInfinite);
            queue<int> Q;
            for (int i = 0; i < size_l_; ++i)
                if (leftMatch[i] == -1) {
                    usedLeft[i] = true;
                    Q.push(i);
                }

            auto updateRight = [&](int node) {
                usedRight[node] = true;
                tillEdge[node] = kInfinite;

                // go left if possible
                if (rightMatch[node] != -1) {
                    usedLeft[rightMatch[node]] = true;
                    Q.push(rightMatch[node]);
                }
            };

            do {
                while (!Q.empty()) {
                    int node = Q.front();
                    Q.pop();

                    // pass the graph
                    for (auto &next : edges_[node])
                        if (not usedRight[next.to]) {
                             if (equal(costLeft[node] + costRight[next.to], next.distance)
                                 && next.to != leftMatch[node])
                                    updateRight(next.to);
                             else
                                tillEdge[next.to] = min(tillEdge[next.to],
                                                        next.distance - costLeft[node] - costRight[next.to]);
                        }
                }

                int nextChange = min_element(tillEdge.begin(), tillEdge.end()) - tillEdge.begin();
                double change = tillEdge[nextChange];
                // we're still stuck here, let's extend
                for (int i = 0; i < size_l_; ++i) {
                    if (usedLeft[i])
                        costLeft[i] += change;
                    if (usedRight[i])
                        costRight[i] -= change;
                    else if (!equal(tillEdge[i], kInfinite))
                        tillEdge[i] -= change;
                }

                if (rightMatch[nextChange] == -1)
                    break;
                updateRight(nextChange);
            } while (true);

            // yey we got a new edge
            used = vector<bool>(size_l_, false);
            for (int i = 0; i < size_l_; ++i)
                if (leftMatch[i] == -1)
                    if (match(i))
                        --steps;
        }

        // and lucky for us the sum of y's is the answer
        double answer = 0;
        for (int i = 0; i < size_l_; ++i)
            answer += costLeft[i] + costRight[i];
        return answer;
    }

  private:
    class Edge {
      public:
        Edge(int _to, double _distance):
                to(_to), distance(_distance) {}
        int to;
        double distance;
    };

    // hopcroft-karp style
    bool match(int node) {
        if (used[node])
            return false;
        used[node] = true;
        for (auto &next : edges_[node])
            if (equal(costLeft[node] + costRight[next.to], next.distance)) {
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
    vector<double> costLeft, costRight;

    vector<bool> used;
};

int main() {
    ifstream cin("adapost.in");
    ofstream cout("adapost.out");

    int N; cin >> N;

    vector<Soldier> A(N);
    vector<Shelter> B(N);

    for (int i = 0; i < N; ++i)
        cin >> A[i].first >> A[i].second;
    for (int i = 0; i < N; ++i)
        cin >> B[i].first >> B[i].second;

    vector< vector<double> > distance(N, vector<double>(N, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            distance[i][j] = sqrt((A[i].first - B[j].first) * (A[i].first - B[j].first) + 
                                  (A[i].second - B[j].second) * (A[i].second - B[j].second));

    vector< pair<int, int> > pairs(N * N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            pairs[i * N + j] = {i, j};
    sort(pairs.begin(), pairs.end(), [&](pair<int, int> A, pair<int, int> B) {
        return distance[A.first][A.second] < distance[B.first][B.second];
    });

    SimpleGraph G(2 * N + 2, 2 * N, 2 * N + 1);
    for (int i = 0; i < N; ++i) {
        G.addEdge(2 * N, i); // source to soldier
        G.addEdge(i + N, 2 * N + 1); // shelter to sink
    }

    double maxDistance = 0;
    for (auto &pair: pairs) {
        G.addEdge(pair.first, pair.second + N);
        if (G.flow() == N) {
            maxDistance = distance[pair.first][pair.second];
            break;
        }
    }

    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(5);
    cout << maxDistance << " ";

    BipartiteGraph M(N, N);
    for (int i = 0; i < N; ++i) {
        for (auto &next : G.edges(i))
            if (next != 2 * N) // if it's not a source edge
                M.addEdge(i, next - N, distance[i][next - N]);
        for (auto &prev : G.edges(i + N))
            if (prev != 2 * N + 1) // if it's not a sink edge
                M.addEdge(prev, i, distance[prev][i]);
    }

    cout << M.minimumWeightMatching() << "\n";
}
