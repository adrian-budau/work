#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <fstream>

using namespace std;

int main() {
    ifstream cin("magicsequence.in");
    ofstream cout("magicsequence.out");

    int T; cin >> T;
    assert(1 <= T && T <= 20);
    while (T--) {
        int N; cin >> N;

        assert(1 <= N && N <= 20 * 1000);
        vector<int> V(N);
        unordered_set<int> H;
        for (int i = 0; i < N; ++i) {
            cin >> V[i];
            assert(1 <= V[i] && V[i] <= 1000 * 1000 * 1000);
            H.insert(V[i]);
        }
        assert(int(H.size()) == N);
        if (V.front() <= V.back())
            cout << "YES\n";
        else
            cout << "NO\n";
    }
}
