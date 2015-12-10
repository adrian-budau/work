#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("colorare2.in");
    ofstream cout("colorare2.out");

    int N, M, K; cin >> N >> M >> K;
    vector<int> from(K), to(K);
    vector<int> degree(N + M, 0);
    for (int i = 0; i < K; ++i) {
        cin >> from[i] >> to[i];
        --from[i]; --to[i];

        ++degree[from[i]];
        ++degree[to[i]];
    }

    int answer = *max_element(degree.begin(), degree.end());

    vector< vector<int> > match(N + M, vector<int>(answer, -1));
    for (int i = 0; i < K; ++i) {
        int x = from[i], y = to[i];

        bool found = false;
        for (int j = 0; j < answer; ++j)
            if (match[x][j] == -1 && match[y][j] == -1) {
                match[x][j] = y;
                match[y][j] = x;
                found = true;
                break;
            }
        if (found)
            continue;

        for (int j = 0; j < answer && !found; ++j)
            if (match[x][j] == -1)
                for (int k = 0; k < answer && !found; ++k)
                    if (match[y][k] == -1) {
                        for (int p = y, col = k; p != -1; p = match[y][col], col = j + k - col)
                            swap(match[p][j], match[p][k]);
                        match[x][j] = y;
                        match[y][j] = x;
                        found = true;
                    }
    }

    cout << answer << "\n";
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < answer; ++j)
            if (match[from[i]][j] == to[i])
                cout << j + 1 << "\n";
}
