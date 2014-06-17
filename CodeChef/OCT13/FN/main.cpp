#include <iostream>
#include <fstream>

using namespace std;

int main() {
    int T; cin >> T;

    while (T--) {
        int N; cin >> N;

        int A = 0, B = 1;
        long long steps = 0;
        do {
            int C = (A + B) % N;
            A = B; B = C;
            ++steps;
            if (steps % 1000000 == 0)
                cerr << " -> " << steps << "\n";
        } while (A != 0 || B != 1);

        cout << steps << "\n";
    }
}
