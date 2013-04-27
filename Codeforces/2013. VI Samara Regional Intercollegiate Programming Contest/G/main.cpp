#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector< vector<int> > edges;
vector<int> state;
vector<int> ts;

bool bad;

void dfs(int node) {
    state[node] = 1;
    for (auto &son: edges[node])
        if (state[son] == 1)
            bad = true;
        else if (state[son] == 0)
            dfs(son);
    state[node] = 2;
    ts.push_back(node);
}

int main() {
    int K, N, M; cin >> K >> N >> M;

    edges.resize(N * M + K);
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < M; ++k) {
                char x; cin >> x;
                if (x == '0')
                    edges[j * M + k].push_back(N * M + i);
                else
                    edges[N * M + i].push_back(j * M + k);
            }

    bad = false;
    state.resize(N * M + K, 0);
    for (int i = 0; i < N * M + K; ++i)
        if (not state[i])
            dfs(i);

    if (bad) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    reverse(ts.begin(), ts.end());

    vector<int> rowanswer(K);
    vector< vector<int> > answer(N, vector<int>(M));

    for (size_t i = 0; i < ts.size(); ++i)
        if (ts[i] < N * M)
            answer[ts[i] / M][ts[i] % M] = i + 1;
        else
            rowanswer[ts[i] - N * M] = i + 1;

    for (auto &row: answer) {
        for (auto &value: row)
            cout << value << " ";
        cout << "\n";
    }
    for (auto &value: rowanswer)
        cout << value << " ";
    cout << "\n";
}
