#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;

vector< vector<int> > edges;
vector<int> dpDown, dpUp;

void dfs(int node, int father = -1) {
    int maxt = -1;
    for (auto &x : edges[node])
        if (x != father) {
            dfs(x, node);
            maxt = max(maxt, dpDown[x]);
        }
    dpDown[node] = maxt + 1;
}

void dfsUp(int node, int up, int father = -1) {
    dpUp[node] = up;
    for (auto &x : edges[node])
        if (x != father) {
            int aux = up;
            for (auto &y : edges[node])
                if (y != father && y != x)
                    aux = max(aux, dpDown[y] + 1);
            dfsUp(x, aux + 1, node);
        }
}

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        edges = vector< vector<int> >(N);
        dpDown = dpUp = vector<int>(N, -1);
        for (int i = 1; i < N; ++i) {
            int x, y; cin >> x >> y;
            edges[x - 1].push_back(y - 1);
            edges[y - 1].push_back(x - 1);
        }

        bool bad = false;
        for (int i = 0; i < N; ++i)
            if (edges[i].size() > 3)
                bad = true;
        if (bad) {
            cout << "-1\n";
            continue;
        }

        dfs(0);
        dfsUp(0, 0);

        int length = N + 5;
        int where = -1;
        for (int i = 0; i < N; ++i)
            if (edges[i].size() < 3 && max(dpDown[i], dpUp[i]) < length) {
                length = max(dpDown[i], dpUp[i]);
                where = i;
            }

        int answer = 1;
        for (int i = 1; i <= length + 1; ++i)
            answer = answer * 2 % kModulo;
        answer = (answer - 1 - N) % kModulo;
        if (answer < 0)
            answer += kModulo;
        cout << where + 1 << " " << answer << "\n";
    }
}
