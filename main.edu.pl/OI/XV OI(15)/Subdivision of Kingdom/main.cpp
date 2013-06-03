#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int N, M; cin >> N >> M;

    vector<int> E(N, 0);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        E[x] |= (1 << y);
        E[y] |= (1 << x);
    }

    vector<int> comb(N, 0);
    for (int i = 0; i < N / 2; ++i)
        comb[i] = 1;

    sort(comb.begin(), comb.end());
    swap(comb[0], comb[N / 2]);
    vector<int> answer = comb;
    int least = M;

    vector<int> bits(1 << 16, 0);
    for (int i = 1; i < (1 << 16); ++i)
        bits[i] = bits[i / 2] + (i % 2);

    do {
        int mask = 0;
        for (int i = 0; i < N; ++i)
            if (comb[i] == 0)
                mask |= (1 << i);
        int aux = 0;
        for (int i = 0; i < N; ++i)
            if (comb[i] == 1) {
                int current = E[i] & mask;
                aux += bits[current & ((1 << 16) - 1)] + bits[current >> 16];
            }

        if (comb[0] == 1 and aux < least) {
            least = aux;
            answer = comb;
        }
    } while (next_permutation(comb.begin(), comb.end()));

    for (int i = 0; i < N; ++i)
        if (answer[i])
            cout << i + 1 << " ";
    cout << "\n";
}
