#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <set>

using namespace std;

int main() {
    int N, X, K; cin >> N >> X >> K;

    multiset<int> A;
    long long B = 0;
    long long sum = 0;
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        if (X > 0) {
            if (x % X)
                A.insert(x % X);
            B += x / X;
        } else
            A.insert(x);
        sum += x;
    }

    if (X == 0) {
        cout << (sum + K - 1) / K << "\n";
        return 0;
    }

    long long answer = 0; 
    if (B > 0) {
        long long total = 1LL * B * K;
        long long took = 0;
        while (total > 0 and A.size()) {
            int v = *A.begin();
            A.erase(A.begin());

            int can = min<long long>(v, total);
            total -= can;
            v -= can;
            took += can;

            if (v > 0)
                A.insert(v);
        }

        if (total > 0) {
            // first we substract what we could
            long long have = 1LL * B * X;

            long long rounds = (took + K - 1) / K;;
            answer += rounds;
            have -= rounds * X;

            if (took % K)
                have -= K - took % K;
            if (have > 0)
                answer += (have + K + X - 1) / (K + X);
        } else
            answer = B;
    }

    while (A.size()) {
        ++answer;
        A.erase(A.find(*A.rbegin()));
 
        int P = K;
        while (P > 0 and A.size()) {
            int v = *A.begin();
            A.erase(A.begin());

            int can = min(v, P);
            v -= can;
            P -= can;

            if (v > 0)
                A.insert(v);
        }
    }

    cout << answer << "\n";
}
