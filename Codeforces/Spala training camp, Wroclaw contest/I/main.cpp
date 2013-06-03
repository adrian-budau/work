#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <cstring>

using namespace std;

int S, D;
const int kMaxN = 1000;

int cap[kMaxN][kMaxN], from[kMaxN];
vector<int> E[kMaxN];

bool bfs() {
    queue<int> Q;
    Q.push(S);
    memset(from, -1, sizeof(from));
    from[S] = S;

    while (!Q.empty()) {
        int x = Q.front();
        Q.pop();

        if (x == D)
            continue;
        for (auto &y : E[x])
            if (cap[x][y] > 0 and from[y] == -1) {
                from[y] = x;
                Q.push(y);
            }
    }

    return from[D] != -1;
}

int main() {
    int N, M; cin >> N >> M;

    vector<string> A(N);

    int K = 0;
    vector< vector< vector<int> > > NODES(N, vector< vector<int> >(M));

    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        for (int j = 0; j < M; ++j)
            if (A[i][j] == 'W')
                NODES[i][j].push_back(++K);
            else if (A[i][j] == 'I') {
                NODES[i][j].push_back(++K);
                NODES[i][j].push_back(++K);
                cap[NODES[i][j][0]][NODES[i][j][1]] = 1;
                E[NODES[i][j][0]].push_back(NODES[i][j][1]);
                E[NODES[i][j][1]].push_back(NODES[i][j][0]);
            } else
                NODES[i][j].push_back(++K);
    }

    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, 1, 0, -1};

    S = 0;
    D = ++K;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            if (A[i][j] == 'W') {
                E[S].push_back(NODES[i][j][0]);
                cap[S][NODES[i][j][0]] = 1;
            }
            if (A[i][j] == 'N') {
                E[NODES[i][j][0]].push_back(D);
                cap[NODES[i][j][0]][D] = 1;
            }

            for (int k = 0; k < 4; ++k) {
                int x = i + dx[k];
                int y = j + dy[k];
                if (x < 0 || x >= N || y < 0 || y >= M)
                    continue;
                if (A[i][j] == 'W' and A[x][y] == 'I') {
                    cap[NODES[i][j][0]][NODES[x][y][0]] = 1;
                    E[NODES[i][j][0]].push_back(NODES[x][y][0]);
                    E[NODES[x][y][0]].push_back(NODES[i][j][0]);
                }

                if (A[i][j] == 'I' and A[x][y] == 'N') {
                    cap[NODES[i][j][1]][NODES[x][y][0]] = 1;
                    E[NODES[i][j][1]].push_back(NODES[x][y][0]);
                    E[NODES[x][y][0]].push_back(NODES[i][j][1]);
                }
            }
        }

    int answer = 0;
    while (bfs()) {
        for (int x = D; x != S; x = from[x]) {
            cap[from[x]][x]--;
            cap[x][from[x]]++;
        }
        ++answer;
    }

    cout << answer << "\n";
}
