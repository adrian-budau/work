#include <iostream>
#include <vector>
#include <map>

using namespace std;

const int kModulo = 1000 * 1000 * 1000 + 7;

int pow(int a, int b) {
    if (b == 0)
        return 1;
    if (b % 2)
        return 1LL * pow(a, b - 1) * a % kModulo;
    int v = pow(a, b / 2);
    return 1LL * v * v % kModulo;
}

int inverse(int x) {
    return pow(x, kModulo - 2);
}

const int NMAX = 20000000;
int precalc[NMAX], largest[NMAX];

map<int, int> memoize;
int f(int p) {
    if (p < NMAX)
        return precalc[p];
    if (memoize.count(p))
        return memoize[p];

    int largest = -1;
    for (int i = 2; i * i <= p; ++i)
        if (p % i == 0) {
            largest = p / i;
            break;
        }
    if (largest == -1)
        largest = 1;
    return (memoize[p] = (f(p - largest) + p) % kModulo);
}

int main() {
    int T; cin >> T;

    for (int i = 1; i < NMAX; ++i)
        for (int j = 2 * i; j < NMAX; j += i)
            largest[j] = max(largest[j], i);
    precalc[1] = 1;
    for (int i = 2; i < NMAX; ++i)
        precalc[i] = (i + precalc[i - largest[i]]) % kModulo;

    for (int test = 1; test <= T; ++test) {
        int K; cin >> K;

        vector< pair<int, int> > primes(K);
        for (int i = 0; i < K; ++i)
            cin >> primes[i].first >> primes[i].second;
        sort(primes.begin(), primes.end());

        vector<int> ways(K, 0), sums(K, 0), sumsWithout(K, 0);
        for (int i = 0; i < K; ++i) {
            ways[i] = primes[i].second + 1;
            sums[i] = 1LL * (pow(primes[i].first, primes[i].second + 1) - 1) * inverse(primes[i].first - 1) % kModulo;
            if (sums[i] < 0)
                sums[i] += kModulo;
            for (int j = 0, pr = 1; j < primes[i].second; ++j) {
                sumsWithout[i] = (sumsWithout[i] + 1LL * pr * (primes[i].second - j)) % kModulo;
                pr = 1LL * pr * primes[i].first % kModulo;
            }
        }

        vector<int> waysPrefix(K, 0), sumsSuffix(K, 0);
        for (int i = 0; i < K; ++i) {
            waysPrefix[i] = ways[i];
            if (i > 0)
                waysPrefix[i] = 1LL * waysPrefix[i - 1] * ways[i] % kModulo;
        }

        for (int i = K - 1; i >= 0; --i) {
            sumsSuffix[i] = sums[i];
            if (i < K - 1)
                sumsSuffix[i] = 1LL * sumsSuffix[i + 1] * sums[i] % kModulo;
        }

        int answer = waysPrefix.back();
        for (int i = 0; i < K; ++i) {
            int aux = 1LL * (f(primes[i].first) - 1) * sumsWithout[i] % kModulo;
            if (aux < 0)
                aux += kModulo;
            if (i > 0)
                aux = 1LL * aux * waysPrefix[i - 1] % kModulo;
            if (i < K - 1)
                aux = 1LL * aux * sumsSuffix[i + 1] % kModulo;
            answer = (answer + aux) % kModulo;
        }

        //int maybe = 0;
        //int number = 1;
        //for (int i = 0; i < K; ++i)
        //    for (int j = 0; j < primes[i].second; ++j)
        //        number *= primes[i].first;
        //for (int i = 1; i <= number; ++i)
        //    if (number % i == 0)
        //        maybe = maybe + precalc[i] % kModulo;
        cout << "Case #" << test << ":\n";
        cout << answer << "\n";
        //cout << "Should be " << maybe << "\n";
    }
}
