#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ifstream cin("input");
    ofstream cout("output");

    int N, M; cin >> N >> M;

    vector< vector<int> > E(N);
    for (int j = 0; j < M; ++j) {
        int x, y; cin >> x >> y;
        E[x].push_back(y);
        E[y].push_back(x);
    }

    int D = (1 << N) - 1;
    vector< pair<int, int> > parent(1 << N, {-1, -1});

    queue<int> Q;
    Q.push(D);
    parent[D] = {D, 0};

    while (!Q.empty()) {
        int state = Q.front();
        Q.pop();

        if (state == 0)
            break;

        for (int i = 0; i < N; ++i) {
            int now_state = state;
            if (now_state & (1 << i))
                now_state -= (1 << i);
            int next_state = 0;
            for (int j = 0; j < N; ++j)
                if ((1 << j) & now_state)
                    for (auto &next : E[j])
                        next_state |= (1 << next);

            if (parent[next_state] == make_pair(-1, -1)) {
                Q.push(next_state);
                parent[next_state] = {state, i};
            }
        }
    }

    if (parent[0] == make_pair(-1, -1)) {
        cout << "Impossible\n";
        return 0;
    }

    for (int i = 0; i < N; ++i)
        cout << 1;
    cout << "\n";

    vector< pair<int, int> > answer;
    for (int i = 0; i != D; i = parent[i].first)
        answer.push_back({i, parent[i].second});
    reverse(answer.begin(), answer.end());
    for (auto &p : answer) {
        if (p.second < 10)
            cout << " ";
        cout << p.second << " -> ";
        for (int j = 0; j < N; ++j)
            if ((1 << j) & p.first)
                cout << 1;
            else
                cout << 0;
        cout << "\n";
    }
}
