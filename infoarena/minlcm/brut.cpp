#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("minlcm.in");
    ofstream cout("minlcm.out");

    int T; cin >> T;
    while (T--) {
        vector<int> minimum(100 * 1000 + 1, 101 * 1000);
        int64_t answer = 1LL * 100 * 1000 * (100 * 1000 - 1);
        int N; cin >> N;
        for (int i = 0; i < N; ++i) {
            int x; cin >> x;
            for (int j = 1; j * j <= x; ++j)
                if (x % j == 0) {
                    answer = min(answer, 1LL * x * minimum[x / j] / (x / j));
                    answer = min(answer, 1LL * x * minimum[j] / j);
                    minimum[x / j] = min(minimum[x / j], x);
                    minimum[j] = min(minimum[j], x);
                }
        }
        cout << answer << "\n";
    }
}
