#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

pair<int64_t, int64_t> dfs(int node, const vector< vector<int> > &dependencies, const vector<int>& time) {
    pair<int64_t, int64_t> answer{time[node], time[node]};
    for (auto &x : dependencies[node]) {
        auto aux = dfs(x, dependencies, time);
        answer.first += aux.first;
        answer.second += 2 * aux.second;
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    vector<int> A(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i];

    vector< vector<int> > dependencies(N);
    for (int i = 0; i < N; ++i) {
        int many; cin >> many;
        for (int j = 0; j < many; ++j) {
            int x; cin >> x;
            dependencies[i].push_back(x - 1);
        }
    }

    auto answer = dfs(0, dependencies, A);
    cout << answer.first << " " << answer.second << "\n";
}
