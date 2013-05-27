#include <cstdio>
#include <vector>

using namespace std;

const int kModulo = 1000000007;

int pow(int x, int y) {
    if (y == 0)
        return 1;
    if (y % 2)
        return (1LL * pow(x, y - 1) * x) % kModulo;
    int v = pow(x, y / 2);
    return (1LL * v * v) % kModulo;
}

int comb(vector<int> &fact, vector<int> &ifact, int N, int K) {
    if (N < K)
        return 0;
    int aux = fact[N];
    aux = (1LL * aux * ifact[K]) % kModulo;
    aux = (1LL * aux * ifact[N - K]) % kModulo;
    return aux;
}

int main() {
    int T;
    scanf("%d", &T);

    vector<int> fact(100001, 0), ifact(100001, 0);
    fact[0] = ifact[0] = 1;
    for (int i = 1; i <= 100000; ++i)
        fact[i] = (1LL * fact[i - 1] * i) % kModulo;
    ifact[100000] = pow(fact[100000], kModulo - 2);
    for (int i = 99999; i > 0; --i)
        ifact[i] = (1LL * ifact[i + 1] * (i + 1)) % kModulo;

    for (int i = 1; i <= T; ++i) {
        long long C;
        int N, K; scanf("%d%lld%d", &N, &C, &K);
        
        int a, b, c; scanf("%d%d%d", &a, &b, &c);
        vector<int> price(N);
        price[0] = a;
        price[1] = b;
        for (int i = 2; i < N; ++i)
            price[i] = 1 + (1LL * a * price[i - 2] + 1LL * b * price[i - 1] + c) % 100000;

        vector<int> pow2(N + 1);
        pow2[0] = 1;
        for (int i = 1; i <= N;  ++i) {
            pow2[i] = pow2[i - 1] * 2;
            if (pow2[i] >= kModulo)
                pow2[i] -= kModulo;
        }

        vector<int> appear(100001, 0);
        for (int i = 0; i < N; ++i)
            appear[price[i]]++;

        vector<int> answer(100001, 0), answer2(100001, 0);
        for (int i = 100000; i > 0; --i) {
            int aux = 0;
            int aux2 = 0;
            int total = appear[i];
            for (int j = i + i; j <= 100000; j += i) {
                aux -= answer[j];
                aux2 -= answer2[j];
                if (aux < 0)
                    aux += kModulo;
                if (aux2 < 0)
                    aux2 += kModulo;

                total += appear[j];
            }

            aux += comb(fact, ifact, total, K);
            if (aux >= kModulo)
                aux -= kModulo;
            aux2 += pow2[total] - 1;
            if (aux2 >= kModulo)
                aux2 -= kModulo;
            if (aux2 < 0)
                aux2 += kModulo;

            answer[i] = aux;
            answer2[i] = aux2;
        }

        int result = 0;
        int result2 = 0;
        for (int i = 1; i <= 100000; ++i) {
            int aux = (C / i) % kModulo;
            result = (result + 1LL * answer[i] * aux) % kModulo;
            result2 = (result2 + 1LL * answer2[i] * aux) % kModulo;
        }

        printf("Case %d: %d %d\n", i, result2, result);
    }
}
