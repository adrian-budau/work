#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000;

int rewrite(vector< pair<int, int> >& E) {
    vector<int> nodes;
    for (auto &p : E) {
        nodes.push_back(p.first);
        nodes.push_back(p.second);
    }
    sort(nodes.begin(), nodes.end());
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
    for (auto &p : E) {
        p.first = lower_bound(nodes.begin(), nodes.end(), p.first) - nodes.begin();
        p.second = lower_bound(nodes.begin(), nodes.end(), p.second) - nodes.begin();
    }

    if (nodes.empty())
        return 1;
    return nodes.size();
}

int formula(int nodes, const vector<int>& components, const vector<int>& powers) {
    int free_nodes = nodes;
    int answer = 1;
    for (auto &x : components) {
        free_nodes -= x;
        answer = 1LL * answer * x % kModulo;
    }

    if (free_nodes == 0 && components.size() == 1)
        return 1;

    return 1LL * answer * powers[components.size() + free_nodes - 2] % kModulo;
}

int dfs(int node, const vector< vector<int> >& edges, vector<bool> &used, int father = -1) {
    used[node] = true;
    int size = 1;
    for (auto &x : edges[node])
        if (x != father) {
            if (used[x])
                return -1;
            int son_size = dfs(x, edges, used, node);
            if (son_size == -1)
                return -1;
            size += son_size;
        }
    return size;
}

int main() {
    ifstream cin("zapezi2.in");
    ofstream cout("zapezi2.out");

    int T; cin >> T;
    while (T--) {
        int N, K; cin >> N >> K;
        vector< pair<int, int> > E(K);
        for (int i = 0; i < K; ++i)
            cin >> E[i].first >> E[i].second;

        vector<int> powers(N);
        powers[0] = 1;
        for (int i = 1; i < N; ++i)
            powers[i] = 1LL * powers[i - 1] * N % kModulo;

        sort(E.begin(), E.end());
        E.erase(unique(E.begin(), E.end()), E.end());
        int nodes = rewrite(E);
        int answer = 0;
        vector< vector<int> > edges(nodes);
        for (int i = 0; i < (1 << E.size()); ++i) {
            int count = 0;
            for (int j = 0; j < nodes; ++j)
                edges[j].clear();
            for (int j = 0; j < int(E.size()); ++j)
                if ((1 << j) & i) {
                    edges[E[j].first].push_back(E[j].second);
                    edges[E[j].second].push_back(E[j].first);
                    ++count;
                }

            vector<bool> used(nodes, false);
            vector<int> components;
            for (int i = 0; i < nodes; ++i)
                if (!used[i]) {
                    components.push_back(dfs(i, edges, used));
                }
            if (find(components.begin(), components.end(), -1) != components.end())
                continue;

            if (count % 2)
                answer = (answer + kModulo - formula(N, components, powers)) % kModulo;
            else
                answer = (answer + formula(N, components, powers)) % kModulo;
         }

        cout << answer << "\n";
    }
}
