#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector< vector< pair<int, int> > > E;
vector<int> used;
vector<int> ts;

void dfs(int node) {
    used[node] = 1;
    for (auto &x : E[node])
        if (not used[x.first])
            dfs(x.first);
    ts.push_back(node);
}

int main() {
    int N, M;
    while (cin >> N >> M) {
        if (N == 0 and M == 0)
            break;
        E.clear();
        E.resize(N);

        vector<int> sum(N, 0);
        for (int i = 0; i < M; ++i) {
            int x, y, z; cin >> x >> y >> z;
            E[x].push_back({y, z});
            sum[x] += z;
        }

        ts.clear();
        used = vector<int>(N, 0);
        dfs(0);
        reverse(ts.begin(), ts.end());

        vector<double> prob(N, 0), expc(N, 0);
        prob[0] = 1;
        for (int i = 0; i < int(ts.size()); ++i)
            for (auto &x : E[ts[i]])
                prob[x.first] += (prob[ts[i]] * x.second) / sum[ts[i]];
        for (int i = ts.size() - 1; i >= 0; --i) {
            for (auto &x : E[ts[i]])
                expc[ts[i]] += ((1.0 + expc[x.first]) * x.second) / sum[ts[i]];
        }

        double answer = expc[0];
        for (int i = 0; i < int(ts.size()); ++i) {
            int from = ts[i];
            if (int(E[from].size()) == 1)
                continue;
            for (auto &x : E[from]) {
                int to = x.first;
                double aux = expc[from] - ((1.0 + expc[to]) * x.second) / sum[from];
                aux /= (1.0 - (1.0 * x.second) / sum[from]);
                aux -= expc[from];
                aux *= prob[from];

                answer = max(answer, aux + expc[0]);
            }
        }

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(10);
        cout << answer << "\n";
    }
}
