#include <iostream>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        cout << "Case #" << test << ":\n";

        int W, R, M, S; cin >> W >> R >> M >> S;

        if (M >= 10) {
            cout << "NOTHING\n";
            continue;
        }

        if (R < 40 || W < 35) {
            cout << "EMERGENCY\n";
            continue;
        }

        if (R > 60) {
            cout << "NIGHTMARE\n";
            continue;
        }

        if (S > 28800) {
            cout << "WAKE-UP\n";
            continue;
        }

        cout << "NOTHING\n";
    }
}
