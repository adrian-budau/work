#include <vector>
#include <iostream>

using namespace std;

int main() {

    vector<int> sieve(1000001, 1);
    sieve[0] = sieve[1] = 1;
    for (int i = 2; i * i <= 1000000; ++i)
        if (sieve[i] == 1) {
            for (int j = i * i; j <= 1000000; j += i)
                sieve[j] = 0;
        }

    for (int i = 1; i <= 1000000; ++i)
        sieve[i] += sieve[i - 1];

    while (true) {
        int A, B; cin >> A >> B;

        if (A == 0 and B == 0)
            break;
        cout << sieve[B] - sieve[A - 1] << "\n";
    }
}
