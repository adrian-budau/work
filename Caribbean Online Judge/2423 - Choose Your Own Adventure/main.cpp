#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

using namespace std;

void dfs(int node, const vector< vector<int> > &edges, const vector<int> &cost, vector<int> &lowest, vector<int> &highest) {
    if (edges[node].empty()) {
        lowest[node] = highest[node] = 0;
    } else {
        for (vector<int>::const_iterator it = edges[node].begin(); it != edges[node].end(); ++it)
            dfs(*it, edges, cost, lowest, highest);

        lowest[node] = cost[edges[node].front()] + lowest[edges[node].front()];
        highest[node] = cost[edges[node].front()] + highest[edges[node].front()];

        for (vector<int>::const_iterator it = edges[node].begin(); it != edges[node].end(); ++it) {
            lowest[node] = min(lowest[node], cost[*it] + lowest[*it]);
            highest[node] = max(highest[node], cost[*it] + highest[*it]);
        }
    }
}

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
         int N; cin >> N;

         map <pair<int, int>, int> Pages;
         vector< vector<int> > edges(N);
         vector<int> cost(N);
         for (int i = 0; i < N; ++i) {
             int a, b, p; cin >> a >> b >> p;
             if (p != -1)
                 edges[p].push_back(i);
             Pages[make_pair(a, b)] = i;
             cost[i] = b - a + 1;
        }

         vector<int> lowest(N, 0), highest(N, 0);
         dfs(0, edges, cost, lowest, highest);

        int Q; cin >> Q;
        cout << "Case " << test << ":\n";
        while (Q--) {
            int page; cin >> page;
            map< pair<int, int>, int>::iterator it = Pages.lower_bound(make_pair(page + 1, page));
            --it;

            int to_add = it -> first.second - page + 1;
            cout << lowest[it -> second] + to_add << " " << highest[it -> second] + to_add << "\n";
        }
    }
}
