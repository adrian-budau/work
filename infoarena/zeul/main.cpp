#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("zeul.in");
    ofstream cout("zeul.out");

    int N, M; cin >> N >> M;
    vector<int> V(N, 0);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        ++V[y];
        --V[x];
    }

    int answer = 0;
    set< pair<int, int> > S;
    for (int i = 0; i < N; ++i) {
        if (V[i] > 0)
            answer += V[i];
        S.emplace(V[i], i);
    }

    cout << answer << "\n";
    while (S.begin()->first != 0) {
        auto p = S.begin()->second;
        auto q = S.rbegin()->second;
        S.erase(make_pair(V[p], p));
        S.erase(make_pair(V[q], q));
        cout << q + 1 << " " << p + 1 << "\n";
        --V[q];
        ++V[p];
        S.emplace(V[p], p);
        S.emplace(V[q], q);
    }
}
