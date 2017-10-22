#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        int M; cin >> M;
        vector< pair<int, int> > res;
        for (int j = 0; j < M; ++j) {
            int x, y; cin >> x >> y;
            if (x + y > N)
                continue;
            res.emplace_back(x, y);
        }

        sort(res.begin(), res.end());
        multiset<int> maxL;
        int best = -1, where = -1;
        for (auto &p : res) {
            maxL.emplace(N - p.second);
            while (maxL.size() && *maxL.begin() < p.first)
                maxL.erase(maxL.begin());
            if (int(maxL.size()) > best) {
                best = maxL.size();
                where = p.first;
            }
        }
        if (where == -1)
            where = 1;
        cout << where << " " << N - where << "\n";
    }
}
