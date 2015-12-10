#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <cassert>
#include <unordered_set>

using namespace std;

int main() {
    ifstream cin("nk.in");
    ofstream cout("nk.out");

    int T; cin >> T;
    assert(1 <= T && T <= 10);

    while (T--) {
        int N, K; cin >> N >> K;
        assert(1 <= N && N <= 1000);
        assert(1 <= K && K <= 100000);

        vector<int> A(N);
        for (int i = 0; i < N; ++i) {
            cin >> A[i];
            assert(0 <= A[i] && A[i] <= 1000 * 1000 * 1000);
        }

        if (K > N) {
            cout << "0\n";
            continue;
        }

        set<int> answer;
        unordered_set<int> have(A.begin(), A.end());
        if (have.count(0))
            answer.insert(0);

        if (K > 12) {
            cout << answer.size() << "\n";
            continue;
        }

        map<int, set<int> > edge;
        vector<int> lo, hi;
        lo.reserve(500);
        hi.reserve(500);
        for (auto x : A) {
            lo.clear();
            hi.clear();
            for (int i = 1; i * i <= x; ++i)
                if (x % i == 0) {
                    lo.push_back(i);
                    hi.push_back(x / i);
                }
            if (lo.back() == hi.back())
                hi.pop_back();
            lo.insert(lo.end(), hi.rbegin(), hi.rend());
            for (auto &x : lo)
                for (auto &y : lo)
                    if (x > y && x % y == 0 && have.count(x / y))
                        edge[y].insert(x / y);
        }

        map<pair<int, int>, int> dp;
        dp[make_pair(1, 1)] = 1;
        if (have.count(1))
            dp[make_pair(1, 1)] |= 2;

        for (auto it = dp.begin(); it != dp.end(); ++it) {
            if ((it->second & (1 << K)) && have.count(it->first.first))
                answer.insert(it->first.first);

            int x = it->first.first;
            for (auto &y : edge[x])
                if (y > it->first.second)
                    dp[make_pair(x * y, y)] |= (it->second << 1);
        }

        cout << answer.size() << "\n";
    }
}
