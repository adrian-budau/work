#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("spectacole.in");
    ofstream cout("spectacole.out");

    int N, M; cin >> N >> M;

    for (int i = 0; i < 2 * N; ++i) {
        int x;
        cin >> x;
    }

    vector< pair<int, int> > V(M);
    for (int i = 0; i < M; ++i) {
        int x; cin >> x >> V[i].first >> V[i].second;
    }

    sort(V.begin(), V.end(), [&](const pair<int, int> &x, const pair<int, int> &y) {
        return x.second < y.second;
    });

    int right = 0;
    int answer = 0;
    for (auto &x : V)
        if (x.first >= right) {
            ++answer;
            right = x.second;
        }
    cout << answer << "\n";
}
