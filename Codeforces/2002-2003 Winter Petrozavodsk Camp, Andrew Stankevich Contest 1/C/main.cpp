#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

using namespace std;

vector< vector<int> > E;
vector< array<int, 2> > answer;
vector<int> from;

void dfs(int node) {
    answer[node][0] = 0;
    answer[node][1] = -1;

    int sum = 0;
    for (auto &fiu: E[node]) {
        dfs(fiu);
        sum += max(answer[fiu][0], answer[fiu][1]);
    }

    answer[node][0] =sum;

    for (auto &fiu : E[node]) {
        int aux = sum - max(answer[fiu][0], answer[fiu][1]);
        aux += answer[fiu][0] + 1;
        if (aux > answer[node][1]) {
            answer[node][1] = aux;
            from[node] = fiu;
        }
    }
}

vector<int> result;

void solve(int node, int type) {
    for (auto &fiu : E[node]) {
        if (type == 1 and from[node] == fiu) {
            result.push_back(fiu);
            solve(fiu, 0);
        } else if (answer[fiu][0] > answer[fiu][1])
            solve(fiu, 0);
        else
            solve(fiu, 1);
    }
}

int main() {
    ifstream cin("grant.in");
    ofstream cout("grant.out");

    int N; cin >> N;
    E.resize(N);
    answer.resize(N);
    from.resize(N);

    for (int i = 1; i < N; ++i) {
        int x; cin >> x; --x;
        E[x].push_back(i);
    }

    dfs(0);

    if (answer[0][0] > answer[0][1]) {
        cout << answer[0][0] * 1000 << "\n";
        solve(0, 0);
    } else {
        cout << answer[0][1] * 1000 << "\n";
        solve(0, 1);
    }

    sort(result.begin(), result.end());
    for (auto &x : result)
        cout << x + 1 << " ";
    cout << "\n";
}
