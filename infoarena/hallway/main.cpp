#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <cassert>
#include <cmath>

using namespace std;

class DisjointSet {
  public:
    DisjointSet(int size = 1):
            set_(size) {
        for (int i = 0; i < size; ++i)
            set_[i] = i;
    }

    int set(int node) {
        if (set_[node] == node)
            return node;
        return (set_[node] = set(set_[node]));
    }

    void join(int first_node, int second_node) {
        first_node = set(first_node);
        second_node = set(second_node);
        set_[first_node] = second_node;
    }

  private:
    vector<int> set_;
};

int main() {
    ifstream cin("hallway.in");
    ofstream cout("hallway.out");

    int N, M; cin >> N >> M;
    int K; cin >> K;

    assert(1 <= N and N <= 1000);
    assert(1 <= M and M <= 1000);
    assert(1 <= K and K <= 1000);

    vector< pair<int, int> > P(K + 1);
    for (int i = 1; i <= K; ++i) {
        cin >> P[i].first >> P[i].second;
        assert(0 <= P[i].first and P[i].first <= N);
        assert(0 <= P[i].second and P[i].second <= M);
    }

    vector< tuple<int, int, int> > edges;
    for (int i = 1; i <= K; ++i) {
        edges.emplace_back(i, 0, P[i].second * P[i].second);
        edges.emplace_back(i, K + 1, (M - P[i].second) * (M - P[i].second));
        for (int j = i + 1; j <= K; ++j)
            edges.emplace_back(i, j, (P[i].first - P[j].first) * (P[i].first - P[j].first) + 
                                     (P[i].second - P[j].second) * (P[i].second - P[j].second));
    }

    sort(edges.begin(), edges.end(), [&](tuple<int, int, int> A, tuple<int, int, int> B) {
        return get<2>(A) < get<2>(B);
    });

    DisjointSet D(K + 2);
    for (auto &p : edges) {
        int x, y, distance;
        tie(x, y, distance) = p;

        D.join(x, y);

        if (D.set(0) == D.set(K + 1)) {
            cout.setf(ios::fixed, ios::floatfield);
            cout.precision(8);
            cout << sqrt(distance) / 2 << "\n";
            return 0;
        }
    }

    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(8);
    cout << M / 2.0 << "\n";
}

