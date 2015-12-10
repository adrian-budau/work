#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;

struct ZP {
    ZP(const int64_t &x = 0):
            v(x % kModulo) {
        if (v < 0)
            v += kModulo;
    }

    ZP operator*(const ZP& that) const {
        return 1LL * v * that.v;
    }

    ZP& operator+=(const ZP& that) {
        v += that.v;
        if (v >= kModulo)
            v -= kModulo;
        return *this;
    }

    ZP& operator-=(const ZP& that) {
        v -= that.v;
        if (v < 0)
            v += kModulo;
        return *this;
    }

    int v;
};

int main() {
    ifstream cin("metrou3.in");
    ofstream cout("metrou3.out");

    int N; cin >> N;
    int64_t A, B; cin >> A >> B;

    vector<int64_t> D(N);
    int64_t T = 0;
    for (int i = 0; i < N; ++i) {
        cin >> D[i];
        T += D[i];
    }

    vector<int64_t> where(N);
    where[0] = 0;
    for (int i = 1; i < N; ++i)
        where[i] = where[i - 1] + D[i - 1];

    vector<int64_t> whenA(N), whenB(N);
    for (int i = 0; i < N; ++i) {
        whenA[i] = A + where[i];
        while (whenA[i] >= T)
            whenA[i] -= T;
    }

    whenB[0] = B;
    for (int i = 1; i < N; ++i) {
        whenB[i] = B + T - where[i];
        while (whenB[i] >= T)
            whenB[i] -= T;
    }

    ZP answer = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            if (i == j)
                continue;
            int64_t timeA = 0, timeB = 0;
            if (where[i] < where[j]) {
                timeA += where[j] - where[i];
                timeB += T - (where[j] - where[i]);
            } else {
                timeB += where[i] - where[j];
                timeA += T - (where[i] - where[j]);
            }

            int64_t wA = whenA[i], wB = whenB[i];
            // simplify cases, A always comes first (or at least as fast as B)
            if (wA > wB) {
                swap(wA, wB);
                swap(timeA, timeB);
            }

            // for the part [0, whenA] the costs will always be timeA + whenA[i] - t
            //                                              and timeB + whenB[i] - t
            answer += ZP(wA + 1) * min(timeA + wA, timeB + wB);

            // for the part (whenA, whenB] the costs will always be timeA + whenA[i] - t + T
            //                                                  and timeB + whenB[i] - t
            answer += ZP(wB - wA) * min(timeA + wA + T, timeB + wB);

            // for the part (whenB, T) the costs will always be timeA + whenA[i] - t + T
            //                                              and timeB + whenB[i] - t + T
            answer += ZP(T - wB - 1) * min(timeA + wA + T, timeB + wB + T);

            // don't forgot to subtract those t, luckily they are all from 0 to T - 1
            answer -= 1LL * T * (T - 1) / 2;
        }

    cout << answer.v << "\n";
}
