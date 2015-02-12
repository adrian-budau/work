#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

const int kMaxN = 100 * 1000 + 5;

vector<int> E[kMaxN];
vector<tuple<int, int, int>> Q[kMaxN];
int A[kMaxN][20];
int answer[kMaxN];
int depth[kMaxN];

void dfsPrepare(int node, int father = 0) {
    A[node][0] = father;
    for (int i = 1; i < 20; ++i)
        A[node][i] = A[A[node][i - 1]][i - 1];

    for (auto &x : E[node])
        if (x != father) {
            depth[x] = depth[node] + 1;
            dfsPrepare(x, node);
        }
}

bool inStack[kMaxN];

int lca(int x, int y) {
    if (depth[x] > depth[y])
        swap(x, y);
    for (int i = 0; i < 20; ++i)
        if ((1 << i) & (depth[y] - depth[x]))
            y = A[y][i];
    if (x == y)
        return x;
    for (int i = 19; i >= 0; --i)
        if ((1 << i) <= depth[x])
            if (A[x][i] != A[y][i]) {
                x = A[x][i];
                y = A[y][i];
            }
    return A[x][0];
}

int stackOf(int x) {
    if (inStack[x])
        return x;
    for (int i = 19; i >= 0; --i)
        if ((1 << i) <= depth[x])
            if (!inStack[A[x][i]])
                x = A[x][i];
    return A[x][0];
}

void dfsSolve(int node, int father = 0) {
    inStack[node] = true;
    for (auto &q : Q[node]) {
        int which = get<0>(q);
        int x = get<1>(q);
        int y = get<2>(q);
        int l = lca(x, y);

        if (!inStack[l]) {
            if (l == x || l == y)
                answer[which] = -1;
            else
                answer[which] = l;
            continue;
        }

        int fromX = stackOf(x);
        int fromY = stackOf(y);
        if (depth[fromX] < depth[fromY])
            fromX = fromY;
        if (fromX == x || fromX == y)
            answer[which] = -1;
        else
            answer[which] = fromX;
    }

    for (auto &x : E[node])
        if (x != father)
            dfsSolve(x, node);
    inStack[node] = false;
}

int main() {
    ios::sync_with_stdio(false);
    int N, M; cin >> N >> M;

    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        E[x].push_back(y);
        E[y].push_back(x);
    }

    dfsPrepare(1);
    int master = 1;
    for (int i = 0; i < M; ++i) {
        int type; cin >> type;
        if (type == 1) {
            int x; cin >> x;
            master = x;
            continue;
        }

        int x, y; cin >> x >> y;
        Q[master].push_back(make_tuple(i, x, y));
    }

    dfsSolve(1);
    for (int i = 0; i < M; ++i)
        if (answer[i])
            cout << answer[i] << "\n";
}
