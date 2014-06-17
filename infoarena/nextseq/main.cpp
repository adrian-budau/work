#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("nextseq.in");
    ofstream cout("nextseq.out");

    int N, M, P; cin >> N >> M >> P;

    vector<int> X(N), A(M), B(P);
    for (int i = 0; i < N; ++i)
        cin >> X[i];
    for (int i = 0; i < M; ++i)
        cin >> A[i];
    reverse(A.begin(), A.end());
    for (int i = 0; i < P; ++i)
        cin >> B[i];
    reverse(B.begin(), B.end());

    sort(X.begin(), X.end());
    map<int, int> digit;
    for (int i = 0; i < N; ++i)
        digit[X[i]] = i + 1;

    for (int i = 0; i < M; ++i)
        A[i] = digit[A[i]];
    A.resize(P, 0);
    for (int i = 0; i < P; ++i)
        B[i] = digit[B[i]];
    int k = 0;

    int answer = 0;
    do {
        ++A[k];
        if (A[k] == N + 1) {
            A[k] = 0;
            ++k;
            continue;
        }

        if (k == 0) {
            if (A == B)
                break;
            ++answer;
            continue;
        }

        --k;
    } while (true);
    cout << answer << "\n";
}
