#include <iostream>

using namespace std;

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        cout << "Case " << test << ": ";

        int X1, Y1, X2, Y2; cin >> X1 >> Y1 >> X2 >> Y2;

        int S = max(max(X1, X2), max(Y1, Y2));
        cout << S - Y1 << " " << X1 << " " << S - Y2 << " " << X2 << "\n";
    }
}
