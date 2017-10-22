#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("minlcm.in");
    ofstream cout("minlcm.out");

    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];

        vector<bool> have(*max_element(V.begin(), V.end()) + 1, false);
        for (auto &x : V)
            have[x] = true;

        int64_t answer = numeric_limits<int64_t>::max();
        for (int d = 1; d < int(have.size()); ++d) {
            int64_t last = numeric_limits<int64_t>::max() / have.size();
            for (int i = d; i < int(have.size()); i += d)
                if (have[i]) {
                    answer = min(answer, last * i / d);
                    last = i;
                }
        }

        cout << answer << "\n";
    }
}
