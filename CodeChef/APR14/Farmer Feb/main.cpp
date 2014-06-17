#include <iostream>

using namespace std;

bool isPrime(int number) {
    for (int i = 2; i * i <= number; ++i)
        if (number % i == 0)
            return false;
    return true;
}

int main() {
    int T; cin >> T;
    while (T--) {
        int X, Y; cin >> X >> Y;
        int Z;
        for (Z = 1; !isPrime(X + Y + Z); ++Z);

        cout << Z << "\n";
    }
}
