#include <iostream>
#include <fstream>
#include <set>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>

using namespace std;

int main() {
    ifstream cin("avioane2.in");
    ofstream cout("avioane2.out");

    int N, M, K; assert(cin >> N >> M >> K);
    assert(1 <= N && N <= 100 * 1000);
    assert(1 <= M && M <= 300 * 1000);
    assert(1 <= K && K <= 200 * 1000);

    vector<set<int> > times(N);
    map< pair<int, int>, int> vertex;
    vector< vector<pair<int, int> > > edges;
    auto index = [&](int A, int B) {
        if (vertex.count(make_pair(A, B)))
            return vertex[make_pair(A, B)];
        int ind = vertex.size();
        vertex[make_pair(A, B)] = ind;
        edges.push_back(vector<pair<int, int> >());
        return ind;
    };

    times[0].emplace(0);
    index(0, 0);
    for (int i = 0; i < M; ++i) {
        int A, start, B, end, price;
        assert(cin >> A >> start >> B >> end >> price);
        assert(1 <= A && A <= N);
        assert(1 <= B && B <= N);
        assert(A != B);
        assert(1 <= start && start < end && end <= 1000 * 1000 * 1000);
        assert(1 <= price && price <= 1000 * 1000 * 1000);
        index(A - 1, start);
        index(B - 1, end);
        edges[index(A - 1, start)].emplace_back(index(B - 1, end), price);
        times[A - 1].insert(start);
        times[B - 1].insert(end);
    }

    for (int i = 0; i < N; ++i) {
        if (times[i].size() <= 1)
            continue;
        for (auto jt = times[i].begin(), it = jt++; jt != times[i].end(); ++it, ++jt) {
            edges[index(i, *it)].emplace_back(index(i, *jt), 0);
        }
    }

    vector<int64_t> dist(edges.size(), numeric_limits<int64_t>::max() / 2);
    dist[index(0, 0)] = 0;
    set< pair<int64_t, int> > D;
    for (int i = 0; i < int(edges.size()); ++i)
        D.emplace(dist[i], i);

    while (!D.empty()) {
        int64_t distance; int node;
        tie(distance, node) = *D.begin();
        D.erase(D.begin());
        for (auto &edge : edges[node])
            if (distance + edge.second < dist[edge.first]) {
                D.erase(make_pair(dist[edge.first], edge.first));
                dist[edge.first] = edge.second + distance;
                D.insert(make_pair(dist[edge.first], edge.first));
            }
    }

    for (int i = 0; i < K; ++i) {
        int node, time; assert(cin >> node >> time);
        assert(1 <= node && node <= N);
        assert(1 <= time && time <= 1000 * 1000 * 1000);
        auto it = times[node - 1].upper_bound(time);
        if (it == times[node - 1].begin())
            cout << "-1\n";
        else {
            --it;
            if (dist[index(node - 1, *it)] >= numeric_limits<int64_t>::max() / 2)
                cout << "-1\n";
            else
                cout << dist[index(node - 1, *it)] << "\n";
        }
    }
}
