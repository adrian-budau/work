#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

const int kInfinite = 0x3f3f3f3f;

vector<pair<int, int> > to_erase;

void dfs(int node, vector< vector< pair<int, int> > > &edges, vector<int> &lowlink, vector<int> &height) {
    lowlink[node] = height[node];

    bool canParent = false;
    for (vector< pair<int, int> >::iterator it = edges[node].begin(); it != edges[node].end(); ++it)
        if (height[it -> first] == -1) {
            height[it -> first] = height[node] + 1;
            dfs(it -> first, edges, lowlink, height);
            lowlink[node] = min(lowlink[node], lowlink[it -> first]);
            if (lowlink[it -> first] == height[it -> first])
                to_erase.push_back(make_pair(node, it -> first));
        } else {
            if (canParent || height[it -> first] != height[node] - 1)
                lowlink[node] = min(lowlink[node], lowlink[it -> first]);
            else
                canParent = true;
        }
}

void eraseEdge(vector< pair<int, int> > &edges, int who) {
    int the_cost = 0;
    for (vector< pair<int, int> >::iterator it = edges.begin(); it != edges.end(); ++it)
        if (it -> first == who)
            the_cost = max(the_cost, it -> second);

    edges.erase(find(edges.begin(), edges.end(), make_pair(who, the_cost)));
}

void dijkstra(int source, vector< vector< pair<int, int> > > &edges, vector<int> &distance) {
    distance = vector<int>(edges.size(), kInfinite);
    distance[source] = 0;

    set< pair<int, int> > S;
    S.insert(make_pair(0, source));

    while (!S.empty()) {
        int x = S.begin() -> second;
        S.erase(S.begin());
        for (vector< pair<int, int> >::iterator it = edges[x].begin(); it != edges[x].end(); ++it)
            if (distance[x] + it -> second < distance[it -> first]) {
                S.erase(make_pair(distance[it -> first], it -> first));
                distance[it -> first] = distance[x] + it -> second;
                S.insert(make_pair(distance[it -> first], it -> first));
            }
    }
}

int main() {
    int T; cin >> T;

    while (T--) {
        int N, M; cin >> N >> M;

        vector< vector< pair<int, int> > > edges(N);
        for (int i = 0; i < M; ++i) {
            int from, to, distance;
            cin >> from >> to >> distance;
            --from; --to;
            edges[from].push_back(make_pair(to, distance));
            edges[to].push_back(make_pair(from, distance));
        }

        vector<int> lowlink(N, 0), height(N, -1);
        height[0] = 0;

        dfs(0, edges, lowlink, height);

        for (vector<pair<int, int> >::iterator it = to_erase.begin(); it != to_erase.end(); ++it) {
            eraseEdge(edges[it -> first], it -> second);
            eraseEdge(edges[it -> second], it -> first);
        }

        to_erase.clear();

        vector<int> left, right;
        dijkstra(0, edges, left);
        dijkstra(N - 1, edges, right);

        int T; cin >> T;
        vector<int> answer;
        for (int i = 1; i + 1 < N; ++i)
            if (left[i] + right[i] <= T)
                answer.push_back(i);

        if (answer.empty())
            cout << "IMPOSSIBLE\n";
        else {
            for (vector<int>::iterator it = answer.begin(); it != answer.end(); ++it)
                cout << *it + 1 << "\n";
        }
    }
}
