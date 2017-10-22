#include <iostream>
#include <vector>

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

int64_t gcd(int64_t a, int64_t b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

vector<int64_t> factorize(int64_t v) {
    vector<int64_t> primes;
    for (int64_t i = 2; i * i <= v; ++i)
        if (v % i == 0) {
            primes.push_back(i);
            while (v % i == 0) {
                v /= i;
            }
        }

    if (v > 1)
        primes.push_back(v);
    return primes;
}

vector<int> powers(vector<int64_t> A, int64_t prime) {
    vector<int> answer;
    for (auto x : A) {
        int power = 0;
        while (x % prime == 0) {
            x /= prime;
            ++power;
        }

        answer.push_back(power);
    }
    return answer;
}

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N, M; cin >> N >> M;
        vector<int64_t> A(N), B(M);
        for (int i = 0; i < N; ++i)
            cin >> A[i];
        for (int j = 0; j < M; ++j)
            cin >> B[j];

        int64_t vA = A[0], vB = B[0];
        for (auto &x : A)
            vA = gcd(vA, x);
        for (auto &y : B)
            vB = gcd(vB, y);

        auto primesA = factorize(vA), primesB = factorize(vB);
        auto primes = primesA; primes.insert(primes.end(), primesB.begin(), primesB.end());
        sort(primes.begin(), primes.end());
        primes.erase(unique(primes.begin(), primes.end()), primes.end());

        int answer = 1;
        for (auto &p : primes) {
            auto powersA = powers(A, p), powersB = powers(B, p);

            int minA = *min_element(powersA.begin(), powersA.end());
            int minB = *min_element(powersB.begin(), powersB.end());
            if (minA == minB) {
                int mulA = 1, mulB = 1;
                for (auto &x : powersA) {
                    if (x == minA)
                        mulA = 1LL * mulA * (minB + 1) % kModulo;
                }

                for (auto &x : powersB) {
                    if (x == minB)
                        mulB = 1LL * mulB * (minA + 1) % kModulo;
                }
                int mul = (mulA + mulB - 1) % kModulo;
                if (mul < 0)
                    mul += kModulo;
                answer = 1LL * answer * mul % kModulo;
                continue;
            }

            if (minA > minB) {
                swap(powersA, powersB);
                swap(minA, minB);
            }

            int mul = 1;
            for (int i = 0; i < int(powersA.size()); ++i)
                if (powersA[i] <= minB)
                    mul = 1LL * mul * (minB + 1)  % kModulo;
            int aux = 1;
            for (auto &y : powersB)
                if (y == minB)
                    aux = 1LL * aux * (minB + 1) % kModulo;
            aux = (aux - 1) % kModulo;
            if (aux < 0)
                aux += kModulo;
            answer = 1LL * answer * (aux + mul) % kModulo;
        }
        cout << "Case #" << test << ":\n";
        cout << answer << "\n";
    }
}
