#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("feast.in");
    ofstream cout("feast.out");

    int T, A, B; cin >> T >> A >> B;
    vector<bool> can(T + 1, false);
    can[0] = true;
    for (int i = 0; i < T; ++i)
        if (can[i]) {
            if (i + A <= T)
                can[i + A] = true;
            if (i + B <= T)
                can[i + B] = true;
        }

    int answer = 0;
    int best = T;
    for (int i = 0; i <= T; ++i) {
        while (best > T - i / 2 || !can[best])
            --best;
        if (can[i])
            answer = max(answer, i / 2 + best);
    }

    cout << answer << "\n";
}
