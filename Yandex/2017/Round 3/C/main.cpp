#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>

using namespace std;

static const int kBase = 43013;
static const int kModulo = 1000 * 1000 * 1000 + 7;

int main() {
    ios::sync_with_stdio(false);

    int N; cin >> N;
    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];
    if (*min_element(V.begin(), V.end()) == *max_element(V.begin(), V.end())) {
        cout << 0 << "\n";
        return 0;
    }

    int step;
    for (step = 1; step < N; step <<= 1);

    int answer;
    // rehash
    map<int, int> M;
    for (int i = 0; i < N; ++i) {
        if (!M.count(V[i])) {
            int sz = M.size();
            M[V[i]] = sz;
        }

        V[i] = M[V[i]];
    }

    for (answer = 0; step; step >>= 1) {
        if (answer + step >= N)
            continue;
        answer += step;
        int power = 1;
        for (int j = 0; j < answer; ++j)
            power = (1LL * power * kBase) % kModulo;

        map<int, int> next;
        int current = 0;
        for (int j = 0; j < answer; ++j)
            current = (1LL * current * kBase + V[j]) % kModulo;
        next[current] = V[answer];
        bool good = true;
        for (int j = answer; j + 1 < N; ++j) {
            current = (1LL * current * kBase + V[j]) % kModulo;
            current = (1LL * current - 1LL * power * V[j - answer]) % kModulo;
            if (current < 0)
                current += kModulo;
            if (next.count(current) && V[j + 1] != next[current]) {
                good = false;
                break;
            }
            next[current] = V[j + 1];
        }
        if (good)
            answer -= step;
    }

    cout << answer + 1 << "\n";
}
