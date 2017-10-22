#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int N, M; cin >> N >> M;
    vector< vector<int> > edges(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        if (x > y)
            swap(x, y);
        edges[y].push_back(x);
    }

    set<int> teams;
    int total_teams = 0;
    vector<int> answer(N);
    for (int i = 0; i < N; ++i) {
        vector<int> deleted;
        for (auto &x : edges[i]) {
            if (teams.count(answer[x])) {
                teams.erase(answer[x]);
                deleted.push_back(answer[x]);
            }
        }
        if (!teams.empty()) {
            answer[i] = *teams.begin();
        } else {
            teams.emplace(total_teams);
            answer[i] = total_teams++;
        }
        for (auto &x : deleted)
            teams.emplace(x);
    }
    cout << total_teams << "\n";
    for (int i = 0; i < N; ++i)
        cout << answer[i] + 1 << " ";
    cout << "\n";
}
