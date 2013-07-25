#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <limits>

using namespace std;

const long long kInfinite = numeric_limits<long long>::max();

vector<int> primes;

const int kMaxP = 65;
long long comb[kMaxP][kMaxP];
long long pows[kMaxP][kMaxP];

vector< pair<long long, long long> > numbers;
void back(long long now, int total, long long ways, int order, int last) {
    if (now > 1)
        numbers.push_back(make_pair(now, ways));
    for (int i = 1; pows[order][i] and i <= last; ++i) {
        if (now > kInfinite / pows[order][i])
            break;
        if (ways > kInfinite / comb[total + i][i])
            continue;
        back(now * pows[order][i], total + i, ways * comb[total + i][i], order + 1, i);
    }
}

int main() {
    comb[0][0] = 1;
    for (int i = 1; i < kMaxP; ++i) {
        comb[i][0] = 1;
        for (int j = 1; j < i; ++j)
            if (comb[i - 1][j] and comb[i - 1][j - 1] and comb[i - 1][j] <= kInfinite - comb[i - 1][j - 1])
                comb[i][j] = comb[i - 1][j] + comb[i - 1][j - 1];
        comb[i][i] = 1;
    }

    for (int i = 2; i <= 100; ++i) {
        bool ok = true;
        for (vector<int>::iterator it = primes.begin(); it != primes.end(); ++it)
            if (i % (*it) == 0)
                ok = false;
        if (ok)
            primes.push_back(i);
    }

    for (int i = 0; i < int(primes.size()); ++i) {
        int x = primes[i];
        pows[i][0] = 1;
        for (int j = 1; j < kMaxP; ++j)
            if (pows[i][j - 1] <= kInfinite / x)
                pows[i][j] = pows[i][j - 1] * x;
            else
                break;
    }

    back(1, 0, 1, 0, 64);
    long long N;

    while (cin >> N) {
        long long answer = kInfinite;
        for (int i = 0; i < int(numbers.size()); ++i)
            if (numbers[i].second == N)
                answer = min(answer, numbers[i].first);
        cout << N << " " << answer << "\n";
    }
}
