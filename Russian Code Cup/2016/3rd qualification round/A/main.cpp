#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    while (T--) {
        int A, B, C; cin >> A >> B >> C;
        int64_t area = 1LL * A * B;
        int64_t low = area / (1LL * C * C), high = low + 1;
        int64_t lowArea = low * C * C, highArea = high * C * C;
        if (lowArea == 0)
            lowArea = -1LL * C * C;
        if (highArea - area < area - lowArea)
            cout << highArea << "\n";
        else
            cout << lowArea << "\n";
    }
}
