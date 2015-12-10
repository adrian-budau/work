#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <map>
#include <set>

using namespace std;

int main() {
    ifstream cin("spectacole.in");
    ofstream cout("spectacole.out");

    int N, M; cin >> N >> M;

    assert(1 <= N && N <= 10* 1000);
    assert(1 <= M && M <= 100 * 1000);

    vector<int> A(N), B(N);
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
        assert(0 <= A[i] && A[i] <= 1000 * 1000 * 1000);
    }

    for (int i = 0; i < N; ++i) {
        cin >> B[i];
        assert(0 <= B[i] && B[i] <= 1000 * 1000 * 1000);
    }

    vector< vector< pair<int, int> > > S(N);
    for (int i = 0; i < M; ++i) {
        int hall, x, y; cin >> hall >> x >> y;
        assert(1 <= hall && hall <= N);
        assert(0 <= x && x < y && y <= 1000 * 1000 * 1000);
        S[hall - 1].emplace_back(x, y);
    }

    for (int i = 0; i < N; ++i) {
        sort(S[i].begin(), S[i].end());
        for (int j = 1; j < int(S[i].size()); ++j)
            assert(S[i][j - 1].second <= S[i][j].first);
    }

    vector< vector< pair<int, int> > > edges(4 * M);
    map<pair<int, int>, int> node;
    auto get = [&](int x, int y) {
        if (node.count(make_pair(x, y)))
            return node[make_pair(x, y)];
        int next = node.size();
        return node[make_pair(x, y)] = next;
    };

    vector<set<int> > times(N + 1);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < int(S[i].size()); ++j) {
            edges[get(S[i][j].first, i)].push_back({get(S[i][j].second, i), 1});
            edges[get(S[i][j].first - B[i], N)].push_back({get(S[i][j].first, i), 0});
            edges[get(S[i][j].second, i)].push_back({get(S[i][j].second + A[i], N), 0});
        }
    }

    for (auto &x : node)
        times[x.first.second].insert(x.first.first);

    for (int i = 0; i <= N; ++i)
        for (auto it = times[i].begin(); it != times[i].end(); ++it) {
            auto jt = it;
            ++jt;
            if (jt == times[i].end())
                break;
            edges[get(*it, i)].emplace_back(get(*jt, i), 0);
        }

    vector<int> dp(edges.size(), 0);
    int answer = 0;
    vector<int> again;
    for (auto &x : node) {
        int now = dp[x.second];
        answer = max(answer, now);
        for (auto &y : edges[x.second]) {
            auto& other = y.first;
            dp[other] = max(dp[other], now + y.second);
        }
        if (x.first.second < N)
            again.push_back(x.second);
        else {
            for (auto &x : again) {
                int now = dp[x];
                for (auto &y : edges[x]) {
                    auto& other = y.first;
                    dp[other] = max(dp[other], now + y.second);
                }
            }
            again.clear();
        }
    }

    cout << answer << "\n";
}
