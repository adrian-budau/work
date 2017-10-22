#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

int main() {
    ifstream cin("steinsgate.in");
    ofstream cout("steinsgate.out");

    int N, M, K; cin >> N >> M >> K;
    vector< vector<int> > edges(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        edges[x - 1].emplace_back(y - 1);
    }

    vector<int> value(N);
    for (int i = 0; i < N; ++i)
        cin >> value[i];

    vector<int> next(N);
    map< vector<int>, int> first_seen;
    for (int i = 0; i < K; ++i) {
        if (first_seen.count(value)) {
            int P = first_seen[value];
            int period = i - P;
            // so it will be seen again at i + period * v
            int need = (K - i) % period;
            for (auto &p : first_seen)
                if (p.second == need + P) {
                    value = p.first;
                    break;
                }
            break;
        } else
            first_seen[value] = i;
        for (int j = 0; j < N; ++j)
            next[j] = 0;
        for (int j = 0; j < N; ++j)
            for (auto &x : edges[j])
                next[x] = max(next[x], value[j]);
        swap(next, value);
    }

    for (int i = 0; i < N; ++i)
        cout << value[i] << " ";
    cout << "\n";
}
