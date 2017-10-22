#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <limits>
#include <map>
#include <cassert>

using namespace std;

template<class T>
int index(map<T, int>& M, T x) {
    if (M.count(x))
        return M[x];
    int size = M.size();
    M[x] = size;
    return size;
}

pair<int, int> find_edge(set< pair<int, int> >& S, int to) {
    auto it = S.lower_bound({to, 0});
    assert(it != S.end());
    auto edge = *it;
    S.erase(it);
    return edge;
}

int flow(vector<set< pair<int, int> >>& edges, int source, int destination) {
    vector<int> from(edges.size(), -1);
    from[source] = source;

    set< pair<int, int> > min_dist;
    min_dist.emplace(0, source);

    vector<int> distance(edges.size(), numeric_limits<int>::max());
    distance[source] = 0;

    while (!min_dist.empty()) {
        int dist, node;
        tie(dist, node) = *min_dist.begin();
        if (dist == numeric_limits<int>::max())
            break;
        min_dist.erase(min_dist.begin());

        for (auto &neighbour : edges[node])
            if (dist + neighbour.second < distance[neighbour.first]) {
                min_dist.erase(make_pair(distance[neighbour.first], neighbour.first));
                distance[neighbour.first] = dist + neighbour.second;
                min_dist.emplace(dist + neighbour.second, neighbour.first);
                from[neighbour.first] = node;
            }
    }

    assert(distance[destination] != numeric_limits<int>::max());

    for (int node = destination; node != source; node = from[node]) {
        auto edge = find_edge(edges[from[node]], node);
        if (node == destination)
            edges[from[node]].emplace(edge.first, edge.second + 1);
        else if (from[node] == source)
            edges[from[node]].emplace(edge.first, edge.second + 1);
        else
            edges[node].emplace(from[node], edge.second);
    }
    return distance[destination];
}

int main() {
    ifstream cin("socks.in");
    ofstream cout("socks.out");

    int N, K; assert(cin >> N >> K);
    assert(1 <= N && N <= 1000);
    assert(1 <= K && K <= 1000);
    map<string, int> colours;
    map<int, int> sizes;

    vector< pair<int, int> > socks;
    for (int i = 0; i < N; ++i) {
        string colour; int size;
        assert(cin >> colour >> size);
        assert(colour.size() <= 20);
        assert(1 <= size && size <= 1000 * 1000 * 1000);
        socks.emplace_back(index(colours, colour), index(sizes, size));
    }

    int source = colours.size() + sizes.size();
    int destination = source + 1;

    vector< set< pair<int, int> > > edges(destination + 1);
    for (auto &sock : socks) {
        edges[sock.first].emplace(sock.second + colours.size(), 0);
    }
    N = colours.size();
    int M = sizes.size();
    for (int i = 0; i < N; ++i)
        edges[source].emplace(i, 0);
    for (int i = N; i < N + M; ++i)
        edges[i].emplace(destination, 0);

    int answer = 0;
    for (int i = 0; i < K; ++i) {
        answer += flow(edges, source, destination);
        cout << answer << "\n";
    }
}
