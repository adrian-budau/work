#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <map>
#include <set>

using namespace std;

template<class T>
bool is_unique(T begin, T end) {
    auto V = vector<int>(begin, end);
    if (V.size() < 2)
        return true;
    sort(V.begin(), V.end());
    for (auto it = V.begin() + 1; it != V.end(); ++it)
        if (*it == *(it - 1))
            return false;
    return true;
}

int main() {
    ifstream cin("compunere.in");
    ofstream cout("compunere.out");

    int N, M; cin >> N >> M;
    assert(1 <= N && N <= 100000);
    assert(1 <= M && M <= 100000);

    vector<int> A(N);
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        assert(-1000 * 1000 * 1000 <= A[i] && A[i] <= 1000 * 1000 * 1000);
    }

    vector<int> B(M);
    for (int i = 0; i < M; ++i) {
        cin >> B[i];
        assert(-1000 * 1000 * 1000 <= B[i] && B[i] <= 1000 * 1000 * 1000);
    }

    assert(is_unique(A.begin(), A.end()));
    assert(is_unique(B.begin(), B.end()));

    map<int, int> whereA;
    for (int i = 0; i < N; ++i)
        whereA[A[i]] = i;

    map<int, int> whereB;
    for (int i = 0; i < M; ++i)
        whereB[B[i]] = i;

    vector<int> longest;
    longest.push_back(1000 * 1000 * 1000 + 1);
    set< pair<int, int>, greater< pair<int, int> > > pairs;
    vector<int> dp(M, 0);
    for (int i = M - 1; i >= 0; --i)
        if (whereA.count(B[i])) {
            auto it = lower_bound(longest.begin(), longest.end(), whereA[B[i]], greater<int>());
            dp[i] = it - longest.begin();
            if (it == longest.end())
                longest.push_back(whereA[B[i]]);
            else
                *it = whereA[B[i]];
            pairs.emplace(dp[i], i);
        }

    int x = 0, y = 0;
    vector<int> answer;
    pairs.insert(make_pair(0, M + 1));
    int best = pairs.begin()->first;
    while (x < N || y < M) {
        if (x == N) {
            answer.push_back(B[y++]);
            continue;
        }
        if (y == M) {
            answer.push_back(A[x++]);
            continue;
        }
        if (A[x] == B[y]) {
            answer.push_back(A[x++]);
            if (dp[y]) {
                pairs.erase(make_pair(dp[y], y));
                dp[y] = 0;
            }
            ++y;
            continue;
        }

        assert(pairs.size());
        int bestSoFar = pairs.begin()->first;
        int bestIfA = bestSoFar, bestIfB = bestSoFar;
        if (dp[y]) {
            pairs.erase(make_pair(dp[y], y));
            assert(pairs.size());
            bestIfB = pairs.begin()->first;
            pairs.insert(make_pair(dp[y], y));
        }
        if (whereB.count(A[x])) {
            int v = whereB[A[x]];
            if (v >= y) {
                pairs.erase(make_pair(dp[v], v));
                assert(pairs.size());
                bestIfA = pairs.begin()->first;
                pairs.insert(make_pair(dp[v], v));
            }
        }

        if ((bestIfA >= bestIfB && A[x] <= B[y]) || bestIfA > bestIfB) {
            if (whereB.count(A[x])) {
                int v = whereB[A[x]];
                if (v >= y) {
                    pairs.erase(make_pair(dp[v], v));
                    dp[v] = 0;
                }
            }
            answer.push_back(A[x++]);
        } else {
            pairs.erase(make_pair(dp[y], y));
            dp[y] = 0;
            answer.push_back(B[y++]);
        }
    }

    assert(int(answer.size()) == M + N - best);
    cout << answer.size() << "\n";
    for (auto &x : answer)
        cout << x << " ";
    cout << "\n";
}
