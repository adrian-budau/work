#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

tuple<int64_t, int64_t, int64_t> gcd(int64_t A, int64_t B) {
    if (B == 0)
        return make_tuple(A, 1, 0);

    int64_t D, X, Y;
    tie(D, X, Y) = gcd(B, A % B);

    return make_tuple(D, Y, X - (A / B) * Y);
}

int main() {
    ifstream cin("antobroasca.in");
    ofstream cout("antobroasca.out");

    int T; cin >> T;
    while (T--) {
        int64_t X, Y, Z, A, B; cin >> X >> Y >> Z >> A >> B;

        assert(1 <= min({X, Y, Z}) and max({X, Y, Z}) <= 1000000000);
        assert(-1000000000 <= min(A, B) and max(A, B) <= 1000000000);

        // X * P1 + Z * Q1 = gcd(X, Z) = d1
        int64_t d1, P1, Q1;

        tie(d1, P1, Q1) = gcd(X, Z);

        // Y * P2 + Z * Q2 = gcd(Y, Z) = d2
        int64_t d2, P2, Q2;
        tie(d2, P2, Q2) = gcd(Y, Z);

        if (A % d1 or B % d2) {
            cout << "NU\n";
            continue;
        }

        P1 *= A / d1;
        Q1 *= A / d1;

        P2 *= B / d2;
        Q2 *= B / d2;

        /**
         * X * P1 + Z * Q1 = A
         * other solutions besides (P1, Q1) are
         *      (P1 + K1 * Z / d1, Q1 - K1 * X / d1)
         * Y * P2 + Z * Q2 = B
         * other solutions besides (P2, Q2) are
         *      (P2 + K2 * Z / d2, Q2 - K2 * Y / d2)
         *
         *
         * Q1 - K1 * X / d1 = Q2 - K2 * Y / d2
         * K2 * Y / d2 - K1 * X / d1 = (Q2 - Q1)
         * K2 * U - K1 * V = Q2 - Q1 (again gcd)
         */
        int64_t U = Y / d2;
        int64_t V = X / d1;
        int64_t K2, K1, d3;
        tie(d3, K2, K1) = gcd(U, V);
        if ((Q2 - Q1) % d3) {
            cout << "NU\n";
            continue;
        }
        cout << "DA\n";
    }
}

