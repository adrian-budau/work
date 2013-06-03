#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool win(vector<long long> A, vector<long long> B) {
    int who = 0;
    while (A.size() and B.size()) {
        if (A.back() >= B.back())
            B.pop_back();
        else
            if (A.size() >= 2) {
                long long x = A.back();
                A.pop_back();
                A.back() += x;
            }
        swap(A, B);
        who = 1 - who;
    }

    if (who == 0)
        return A.size() > 0;
    return B.size() > 0;
}

int main() {
    int N, M, test;
    test = 0;
    while (cin >> N >> M) {
        test++;

        vector<long long> A(N), B(M);
        for (int i = 0; i < N; ++i)
            cin >> A[i];
        for (int j = 0; j < M; ++j)
            cin >> B[j];

        sort(A.begin(), A.end());
        sort(B.begin(), B.end());

        bool ok = true;
        if (A.back() >= B.back()) {
            vector<long long> C = B;
            C.pop_back();
            if (not win(C, A))
                ok = false;
        }

        if (A.size() >= 2) {
            vector<long long> C = A;
            long long x = C.back();
            C.pop_back();
            C.back() += x;
            if (not win(B, C))
                ok = false;
        }

        cout << "Case " << test << ": ";
        if (ok)
            cout << "Buyout Limited\n";
        else
            cout << "Takeover Incorporated\n";
    }
}
