#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int64_t gcd(int64_t A, int64_t B) {
    if (B == 0)
        return A;
    return gcd(B, A % B);
}

int main() {
    ifstream cin("criptare.in");
    ofstream cout("criptare.out");

    int N, M; cin >> N >> M;

    vector<int> B(N);
    for (int i = 0; i < N; ++i)
        cin >> B[i];

    vector<int64_t> A(N, 0);
    vector<bool> used(N, false);
    for (int i = 0; i < N; ++i)
        if (not used[i]) {
            int64_t now = B[(i + 1) % N] - B[i];
            int64_t minimum = 0;
            for (int j = (i + M) % N; j != i; j = (j + M) % N) {
                minimum = min(minimum, now);
                now += B[(j + 1) % N] - B[j];
            }

            now = -minimum;
            for (int j = i; not used[j]; j = (j + M) % N) {
                A[j] = now;
                used[j] = true;
                now += B[(j + 1) % N] - B[j];
            }
        }
    int K = gcd(N, M);
    int64_t diff = B[0];
    for (int j = 0; j < M; ++j)
        diff -= A[j % N];

    diff /= M / K;
    A[K - 1] += diff;
    for (int j = (K - 1 + M) % N; j != K - 1; j = (j + M) % N)
        A[j] += diff;

    for (int i = 0; i < N; ++i)
        cout << A[i] << " ";
    cout << "\n";
}
