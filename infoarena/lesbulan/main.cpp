#include <iostream>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

bool dfs(int node, const vector< set<int> > &E, vector<bool> &used) {
    used[node] = true;
    if (E[node].size() > 2)
        return true;
    for (auto &next : E[node])
        if (not used[next])
            if (dfs(next, E, used))
                return true;
    return false;
}

int main() {
    ifstream cin("lesbulan.in");
    ofstream cout("lesbulan.out");

    int T; cin >> T;

    while (T--) {
        int N, M; cin >> N >> M;

        vector< set<int> > E(N);
        for (int i = 0; i < M; ++i) {
            int x, y; cin >> x >> y;
            --x; --y;
            E[x].insert(y);
            E[y].insert(x);
        }

        for (int i = 0; i < 2; ++i) {
            vector<int> toRemove;
            for (int j = 0; j < N; ++j)
                if (E[j].size() == 1)
                    toRemove.push_back(j);
            for (auto &node : toRemove) {
                if (E[node].empty())
                    continue;
                int next = *E[node].begin();
                E[node].erase(next);
                E[next].erase(node);
            }
        }

        // now we must have only chains left
        bool badGraph = false;
        vector<bool> used(N, false);
        for (int i = 0; i < N; ++i)
            if (not used[i] and E[i].size() < 2)
                badGraph |= dfs(i, E, used);

        badGraph |= (used != vector<bool>(N, true));

        if (badGraph)
            cout << "0\n";
        else
            cout << "1\n";
    }
}
