#include <iostream>
#include <set>

using namespace std;

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N, X; cin >> N >> X;

        multiset<int> S;
        for (int i = 0; i < N; ++i) {
            int x;
            cin >> x;
            S.insert(x);
        }

        int discs = 0;
        while (S.size()) {
            int x = *S.rbegin();
            S.erase(S.find(x));

            auto it = S.upper_bound(X - x);
            if (it != S.begin()) {
                --it;
                S.erase(it);
            }
            ++discs;
        }

        cout << "Case #" << test << ": " << discs << "\n";
    }
}

