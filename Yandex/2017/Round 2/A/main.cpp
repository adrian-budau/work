#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int abs(int x) {
    if (x < 0)
        return -x;
    return x;
}

int main() {
    int N, K; cin >> N >> K;

    if (K == 0) {
        cout << "Yes\n";
        cout << abs(N) + 1 << " " << abs(N) + 2 << "\n";
        return 0;
    }
    if (N == 0) {
        if (K == 1) {
            cout << "Yes\n";
            cout << "0 1\n";
            return 0;
        }
        cout << "No\n";
        return 0;
    }

    if (K == 1) {
        cout << "Yes\n";
        cout << N << " " << 2 * N << "\n";
        return 0;
    }

    if (K == 2) {
        cout << "Yes\n";
        cout << N << " " << N << "\n";
        return 0;
    }

    if (K == 3) {
        cout << "Yes\n";
        cout << N << " " << 0 << "\n";
        return 0;
    }

    cout << "No\n";
}
