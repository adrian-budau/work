#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);

    double SZ; cin >> SZ;
    cout << 0 << " " << 0 << "\n";
    cout.flush();

    double x, y;
    while (cin >> x >> y) {
        cout.setf(ios::fixed, ios::floatfield);
        cout << -x << " " << -y << "\n";
        cout.flush();
    }
}
