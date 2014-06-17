#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("ninja.in");
    ofstream cout("ninja.out");

    int N, M, K; cin >> N >> M >> K;

    vector< vector<int> > V(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        V[(x + y) / 2].push_back((y - x) / 2 + 1);
    }

    for (int i = 0; i < K; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;

        int64_t answer = 0;
        for (int j = x; j <= y; ++j)
            for (auto &wide : V[j])
                answer += min({wide, j - x + 1, y - j + 1});

        cout << answer << "\n";
    }
}
