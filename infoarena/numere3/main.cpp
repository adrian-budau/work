#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("numere3.in");
    ofstream cout("numere3.out");

    int N, M, K; cin >> N >> M >> K;
    vector< vector<int> > A(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            cin >> A[i][j];
            A[i][j] %= 2;
        }
    vector< vector<bool> > bad(N, vector<bool>(M, false));
    for (int i = 0; i < K; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        bad[x][y] = true;
    }

    vector< pair<int, int> > answer;
    for (int step = 0; step < 2; ++step) {
        auto B = A;
        for (int i = 0; i < (1 << M); ++i) {
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < M; ++j)
                    B[i][j] = A[i][j];
            vector< pair<int, int> > presses;
            auto press = [&](int x, int y) {
                int neighbours = 4;
                if (x == 0)
                    --neighbours;
                else
                    B[x - 1][y] ^= 1;
                if (x == N - 1)
                    --neighbours;
                else
                    B[x + 1][y] ^= 1;
                if (y == 0)
                    --neighbours;
                else
                    B[x][y - 1] ^= 1;
                if (y == M - 1)
                    --neighbours;
                else
                    B[x][y + 1] ^= 1;

                if (neighbours % 2)
                    B[x][y] ^= 1;
                presses.emplace_back(x, y);
            };
            bool ok = true;
            for (int j = 0; j < M; ++j)
                if ((1 << j) & i) {
                    press(0, j);
                    if (bad[0][j])
                        ok = false;
                }
            if (!ok)
                continue;

            for (int i = 1; i < N; ++i)
                for (int j = 0; j < M; ++j)
                    if (B[i - 1][j]) {
                        if (bad[i][j])
                            ok = false;
                        press(i, j);
                    }
            if (!ok)
                continue;
            if (B.back() == vector<int>(M, 0))
                if (answer.empty() || presses.size() < answer.size())
                    answer = presses;
        }
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                A[i][j] ^= 1;
    }
    if (answer.empty()) {
        cout << "-1\n";
        return 0;
    }

    cout << answer.size() << "\n";
    for (auto &p : answer)
        cout << p.first + 1 << " " << p.second + 1 << "\n";
}
