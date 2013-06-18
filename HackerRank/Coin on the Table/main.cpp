#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <tuple>
#include <queue>

using namespace std;

int main() {
    int N, M, K; cin >> N >> M >> K;

    vector< vector<int> > A(N, vector<int>(M, -1));
    int X = 0, Y = 0;
    for (int i = 0; i < N; ++i) {
        string S; cin >> S;
        for (int j = 0; j < M; ++j) {
            switch (S[j]) {
                case 'U':
                    A[i][j] = 0;
                    break;
                case 'L':
                    A[i][j] = 1;
                    break;
                case 'D':
                    A[i][j] = 2;
                    break;
                case 'R':
                    A[i][j] = 3;
                    break;
                default:
                    X = i, Y = j;
                    break;
            }
        }
    }

    array<int, 4> dx = {-1, 0, 1, 0}, dy = {0, -1, 0, 1};

    queue< tuple<int, int, int> > current, next;
    vector< vector< vector<bool> >> used(N, vector< vector<bool> >(M, vector<bool>(K + 1, 0)));

    next.push(tuple<int, int, int>{0, 0, 0});
    used[0][0][0] = 1;

    int cost = 0;
    while (true) {
        swap(next, current);
        next = queue<tuple<int, int, int>>();

        if (current.empty())
            break;
        queue< tuple<int, int, int> > aux;
        while (not current.empty()) {
            auto now = current.front();
            current.pop();

            aux.push(now);

            int x, y, t;
            tie(x, y, t) = now;
            if (x == X and y == Y and t <= K) {
                cout << cost << "\n";
                return 0;
            }

            int op = A[x][y];
            x += dx[op];
            y += dy[op];
            t += 1;
            if (t > K)
                continue;
            if (x >= 0 and x < N and y >= 0 and y < M and used[x][y][t] == 0) {
                used[x][y][t] = 1;
                current.push(tuple<int, int, int>{x, y, t});
            }
        }

        swap(current, aux);

        while (not current.empty()) {
            auto now = current.front();
            current.pop();

            int x, y, t;
            tie(x, y, t) = now;

            for (int i = 0; i < 4; ++i) {
                int x2 = x + dx[i];
                int y2 = y + dy[i];
                int t2 = t + 1;
                if (t2 > K)
                    continue;
                if (x2 >= 0 and x2 < N and y2 >= 0 and y2 < M and used[x2][y2][t2] == 0) {
                    used[x2][y2][t2] = 1;
                    next.push(tuple<int, int, int>{x2, y2, t2});
                }
            }
        }

        ++cost;
    }

    cout << "-1\n";
}
