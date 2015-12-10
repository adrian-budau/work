#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;

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

    int64_t answer = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j)
                for (int t = 0; t < T; ++t) {
                    int64_t timeA = 0, timeB = 0;
                    if (where[i] < where[j]) {
                        timeA += where[j] - where[i];
                        timeB += T - (where[j] - where[i]);
                    } else {
                        timeB += where[i] - where[j];
                        timeA += T - (where[i] - where[j]);
                    }
                    if (t <= whenA[i])
                        timeA += whenA[i] - t;
                    else
                        timeA += whenA[i] - t + T;
                    if (t <= whenB[i])
                        timeB += whenB[i] - t;
                    else
                        timeB += whenB[i] - t + T;
                    answer += min(timeA, timeB);
                }

    cout << answer % kModulo<< "\n";
}
