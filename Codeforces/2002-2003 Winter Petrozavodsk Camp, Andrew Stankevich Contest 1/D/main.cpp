#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("matrix.in");
    ofstream cout("matrix.out");

    int N, M; cin >> N >> M;
    vector<int> degree(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        ++degree[x]; ++degree[y];
    }

    int64_t answer = 0;
    for (int i = 0; i < N; ++i)
        answer += int64_t(degree[i]) * degree[i];
    cout << answer << "\n";
}
