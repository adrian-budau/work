#include <iostream>
#include <vector>

using namespace std;

const int kModulo = 1000000009;

int main() {
    int N; cin >> N;

    vector<int> lowest_prime(N + 1, 0);
    vector<int> primes;
    for (int i = 2; i <= N; ++i) {
        if (lowest_prime[i] == 0) {
            lowest_prime[i] = i;
            primes.push_back(i);
        }

        for (int j = 0; j < int(primes.size()) and primes[j] <= lowest_prime[i] and i * primes[j] <= N; ++j)
            lowest_prime[i * primes[j]] = primes[j];
    }

    vector<int> order(N + 1, 0);
    for (int i = 2; i <= N; ++i)
        for (int j = i; j != 1; j /= lowest_prime[j])
            ++order[lowest_prime[j]];

    int answer = 1;
    for (int i = 2; i <= N; ++i) {
        if (not order[i])
            continue;
        int aux = 1;
        for (int j = 0; j < order[i]; ++j)
            aux = (static_cast<int64_t>(aux) * i) % kModulo;

        aux = (static_cast<int64_t>(aux) * aux) % kModulo;
        answer = (static_cast<int64_t>(answer) * (aux + 1)) % kModulo;
    }

    cout << answer << "\n";
}
