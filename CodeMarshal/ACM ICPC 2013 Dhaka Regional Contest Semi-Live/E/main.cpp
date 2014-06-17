#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

const int kModulo = 1000003;

int pow(int a, int b) {
    if (b == 0)
        return 1;
    if (b % 2)
        return (1LL * pow(a, b - 1) * a) % kModulo;
    int value = pow(a, b / 2);
    return (1LL * value * value) % kModulo;
}

int main() {
    int T; cin >> T;

    vector<int> fact(100000);
    fact[0] = 1;
    for (int i = 1; i < 100000; ++i)
        fact[i] = (1LL * fact[i - 1] * i) % kModulo;

    vector<int> ifact(100000);
    for (int i = 0; i < 100000; ++i)
        ifact[i] = pow(fact[i], kModulo - 2);

    while (T--) {
        int A, B, X, Y, Z; cin >> A >> B >> X >> Y >> Z;

        int answer = 0;
        for (int type3 = 0; type3 <= A && type3 <= B; ++type3) {
            int type1 = A - type3;
            int type2 = B - type3;

            int aux = (1LL * fact[type1 + type2 + type3] * ifact[type1]) % kModulo;
            aux = (1LL * aux * ifact[type2]) % kModulo;
            aux = (1LL * aux * ifact[type3]) % kModulo;
            aux = (1LL * aux * pow(X, type1)) % kModulo;
            aux = (1LL * aux * pow(Y, type2)) % kModulo;
            aux = (1LL * aux * pow(Z, type3)) % kModulo;
            answer = (answer + aux) % kModulo;
        }

        cout << answer << "\n";
    }
}
