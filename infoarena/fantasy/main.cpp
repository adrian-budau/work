#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <limits>
#include <queue>
#include <algorithm>

using namespace std;

pair< vector<int>, vector<int> > bfs(const vector< vector<int> > &edges, int start) {
    queue<int> Q;
    vector<int> dist(edges.size(), numeric_limits<int>::max());
    vector<int> from(edges.size(), -1);

    Q.push(start);
    dist[start] = 0;
    from[start] = start;
    while (!Q.empty()) {
        int node = Q.front();
        Q.pop();
        for (auto &x : edges[node])
            if (dist[x] == numeric_limits<int>::max()) {
                dist[x] = dist[node] + 1;
                from[x] = node;
                Q.push(x);
            }
    }
    return make_pair(dist, from);
}

int main() {
    ifstream cin("fantasy.in");
    ofstream cout("fantasy.out");

    int T; assert(cin >> T);
    assert(1 <= T && T <= 100);
    while (T--) {
        int N, D, K, W;
        assert(cin >> N >> D >> K >> W);
        assert(3 <= N && N <= 1000);
        assert(1 <= D && D <= N);
        assert(1 <= K && K <= N);
        assert(1 <= W && W <= N);
        assert(D != K && K != W && D != W);

        --D; --K; --W;
        vector< vector<int> > edges(N);
        for (int i = 1; i < N; ++i) {
            int x, y; assert(cin >> x >> y);
            assert(1 <= x && x <= N);
            assert(1 <= y && y <= N);
            assert(x != y);
            --x; --y;
            edges[x].push_back(y);
            edges[y].push_back(x);
        }

        vector<int> distW, distD, distK, from;
        tie(distW, from) = bfs(edges, 0);
        assert(*max_element(distW.begin(), distW.end()) != numeric_limits<int>::max());

        tie(distW, from) = bfs(edges, W);
        tie(distD, from) = bfs(edges, D);
        tie(distK, from) = bfs(edges, K);
        vector<bool> bad(N, false);
        for (int i = D; i != K; i = from[i])
            bad[i] = true;
        bad[K] = true;
        bool found = false;
        for (int i = 0; i < N; ++i)
            if (!bad[i] && distW[i] < distK[i] && distK[i] <= distD[i]) {
                cout << "DA\n";
                found = true;
                break;
            }
        if (!found)
            cout << "NU\n";
    }
}
