#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>

using namespace std;

void solve(vector<int> left, vector< vector<int> > &E, vector<int> &answer, int offset = 0) {
    if (left.size() == 0)
        return;
    if (left.size() == 1) {
        answer[left[0]] = offset;
        return;
    }

    int node = *min_element(left.begin(), left.end());
    vector<int> used(E.size(), 0);
    queue<int> Q;
    Q.push(node);
    used[node] = 1;

    while (!Q.empty()) {
        int x = Q.front();
        Q.pop();

        for (auto &y : E[x])
            if (not used[y]) {
                used[y] = 1;
                Q.push(y);
            }
    }

    vector<int> newLeft, newRight;
    for (auto &x : left)
        if (x != node) {
            if (used[x])
                newLeft.push_back(x);
            else
                newRight.push_back(x);
        }
    answer[node] = offset + newLeft.size();
    solve(newLeft, E, answer, offset);
    solve(newRight, E, answer, offset + 1 + newLeft.size());
}

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N; cin >> N;

        vector<int> A(N), B(N);
        for (int i = 0; i < N; ++i)
            cin >> A[i];
        for (int j = 0; j < N; ++j)
            cin >> B[j];

        vector< vector<int> > E(N);
        vector<int> last(N + 1, -1);
        for (int i = 0; i < N; ++i) {
            if (last[A[i]] != -1)
                E[last[A[i]]].push_back(i);
            if (last[A[i] - 1] != -1)
                E[i].push_back(last[A[i] - 1]);
            last[A[i]] = i;
        }

        last = vector<int>(N + 1, -1);
        for (int i = N - 1; i >= 0; --i) {
            if (last[B[i]] != -1)
                E[last[B[i]]].push_back(i);
            if (last[B[i] - 1] != -1)
                E[i].push_back(last[B[i] - 1]);
            last[B[i]] = i;
        }

        vector<int> answer(N, 0);
        vector<int> left(N);
        for (int i = 0; i < N; ++i)
            left[i] = i;
        solve(left, E, answer);

        cout << "Case #" << test << ": ";
        for (auto &x : answer)
            cout << x + 1 << " ";
        cout << "\n";
    }
}

