#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>

using namespace std;

bool solve(int leaf, int father, vector< vector<int> > &E, vector< vector<bool> > &has, vector<int>& answer) {
    for (int i = 0; i < int(E.size()); ++i)
        answer[i] = -1;
    answer[leaf] = father;

    queue< pair<int, int> > Q;
    Q.emplace(leaf, father);
    vector<bool> used(E.size(), false);
    used[leaf] = used[father] = true;
    while (!Q.empty()) {
        int father, node;
        tie(father, node) = Q.front();
        Q.pop();
        for (auto &x : E[node])
            if (!used[x])
                if (has[father][x]) {
                    answer[x] = node;
                    used[x] = true;
                    Q.emplace(node, x);
                }
    }

    if (count(answer.begin(), answer.end(), -1) != 1)
        return false;
    for (int i = 0; i < int(E.size()); ++i)
        for (int j = i + 1; j < int(E.size()); ++j) {
            bool has_real = false;
            if (answer[i] == j)
                has_real = true;
            if (answer[j] == i)
                has_real = true;
            if (answer[i] == answer[j])
                has_real = true;
            if (answer[i] != -1 && answer[answer[i]] == j)
                has_real = true;
            if (answer[j] != -1 && answer[answer[j]] == i)
                has_real = true;
            if (has_real != has[i][j])
                return false;
        }
    return true;
}

int main() {
    ifstream cin("tree2.in");
    ofstream cout("tree2.out");

    int N, M; cin >> N >> M;
    vector< vector<int> > E(N);
    vector< vector<bool> > has(N, vector<bool>(N, false));
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        E[x - 1].push_back(y - 1);
        E[y - 1].push_back(x - 1);
        has[x - 1][y - 1] = has[y - 1][x - 1] = true;
    }

    vector<int> pos(N);
    for (int i = 0; i < N; ++i)
        pos[i] = i;
    random_shuffle(pos.begin(), pos.end());

    vector<int> answer(N);
    for (auto &leaf : pos) {
        for (auto &father : E[leaf]) {
            if (solve(leaf, father, E, has, answer)) {
                for (int i = 0; i < N; ++i)
                    if (answer[i] != -1)
                        cout << i + 1 << " " << answer[i] + 1 << "\n";
                return 0;
            }
        }
    }
}
