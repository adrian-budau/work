#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

const int64_t kPrime = 31;
const int64_t kModulo = (1LL << 59) - 1;

int main() {
    ifstream cin("potriveala.in");
    ofstream cout("potriveala.out");

    string A, B; cin >> A >> B;
    int M = B.size();
    while (B.size() < 2 * A.size())
        B += B;

    for (auto &ch : A)
        ch -= 'A' - 1;
    for (auto &ch : B)
        ch -= 'A' - 1;

    int step = 0;
    for (step = 1; step < int(A.size()); step <<= 1);

    int answer;
    for (answer = 0; step; step >>= 1) {
        if (answer + step > int(A.size()))
            continue;
        answer += step;

        int64_t precalc = 1;
        for (int i = 0; i < answer; ++i)
            precalc = (precalc * kPrime) & kModulo;
        int64_t hash = 0;
        unordered_set<int64_t> H;
        H.rehash(M);
        for (int j = 0; j < M + answer and j < int(B.size()); ++j) {
            hash = (hash * kPrime + B[j]) & kModulo;
            if (j >= answer)
                hash = (hash - precalc * (B[j - answer])) & kModulo;

            if (j >= answer - 1)
                H.insert(hash);
        }

        hash = 0;
        bool ok = false;
        for (int j = 0; j < int(A.size()); ++j) {
            hash = (hash * kPrime + A[j]) & kModulo;
            if (j >= answer)
                hash = (hash - precalc * (A[j - answer])) & kModulo;

            if (j >= answer - 1)
                if (H.count(hash)) {
                    ok = true;
                    break;
                }
        }

        if (!ok)
            answer -= step;
    }

    cout << answer << "\n";
}
