#include <iostream>

using namespace std;

void solve(int N) {
    if (N < 8)
        return;
    int x = (N - 1) * (N - 2) * (N - 3);

    x /= 6;
    cout << "*" << x;
    solve(N - 4);
}

int main() {
    int N, R; cin >> N >> R;

    cout << "(";

    cout << "1";
    solve(N);

    cout << ")^" << R << "\n";
}
