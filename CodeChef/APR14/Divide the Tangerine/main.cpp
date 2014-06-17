#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool bad(pair<int, int> A, pair<int, int> B, int N) {
    if (A.second < A.first)
        A.second += N;
    if (B.second < B.first)
        B.second += N;

    if (A.second < B.first) {
        A.first += N;
        A.second += N;
    } else if (B.second < A.first) {
        B.first += N;
        B.second += N;
    }

    if (B.first <= A.first && A.second <= B.second)
        return false;
    if (A.first <= B.first && B.second <= A.first)
        return true;
    if (B.second < A.first)
        return false;
    if (A.second < B.first)
        return false;
    return true;
}

int main() {
    int T; cin >> T;
    while (T--) {
        int N, K, P; cin >> N >> K >> P;
        vector< pair<int, int> > A(K), B(P);
        for (int i = 0; i < K; ++i)
            cin >> A[i].first >> A[i].second;
        for (int i = 0; i < P; ++i)
            cin >> B[i].first >> B[i].second;

        int answer = true;
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < P; ++j)
                if (bad(A[i], B[j], N))
                    answer = false;
        if (answer)
            cout << "Yes\n";
        else
            cout << "No\n";
    }
}
