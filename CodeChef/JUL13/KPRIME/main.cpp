#include <iostream>
#include <vector>

using namespace std;

const int kMaxValue = 1000000;

int main() {
    vector<int> primes(kMaxValue + 1, 0);
    vector< vector<int> > many(6, vector<int>(kMaxValue + 1, 0));

    for (int i = 2; i <= kMaxValue; ++i) {
        if (primes[i] == 0)
            for (int j = i; j <= kMaxValue; j += i)
                ++primes[j];

        for (int j = 0; j < 6; ++j)
            many[j][i] = many[j][i - 1];
        if (primes[i] <= 5)
            ++many[primes[i]][i];
    }

    int T; cin >> T;
    while (T--) {
        int A, B, K; cin >> A >> B >> K;

        cout << many[K][B] - many[K][A - 1] << "\n";
    }
}
