#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

bool canReachAll(vector<int> stack, const vector< vector<bool> > &edge, vector<bool> used) {
    if (stack.empty())
        return false;

    queue<int> Q;
    for (auto nod : stack)
        Q.push(nod);

    while (!Q.empty()) {
        int nod = Q.front();
        Q.pop();

        for (int i = 0; i < int(edge.size()); ++i)
            if (edge[nod][i] and not used[i]) {
                used[i] = true;
                Q.push(i);
            }
    }

    return find(used.begin(), used.end(), false) == used.end();
}

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N, M; cin >> N >> M;
        vector<string> S(N);
        for (int i = 0; i < N; ++i)
            cin >> S[i];

        vector< vector<bool> > edge(N, vector<bool>(N, false));
        for (int i = 0; i < M; ++i) {
            int x, y; cin >> x >> y;
            --x; --y;
            edge[x][y] = edge[y][x] = true;
        }

        vector<bool> used(N, false);

        int start = min_element(S.begin(), S.end()) - S.begin();
        vector<int> stack;

        used[start] = true;
        stack.push_back(start);

        string answer = S[start];
        for (int step = 1; step < N; ++step) {
            auto now = stack;

            vector<int> considered;
            while (canReachAll(now, edge, used)) {
                considered.push_back(now.back());
                now.pop_back();
            }

            string best = "9999999";
            int where = -1;
            for (auto nod : considered)
                for (int i = 0; i < N; ++i)
                    if (edge[nod][i] and not used[i])
                        if (S[i] < best) {
                            best = S[i];
                            where = i;
                        }

            answer += best;
            while (not edge[stack.back()][where])
                stack.pop_back();

            used[where] = true;
            stack.push_back(where);
        }
        cout << "Case #" << test << ": " << answer << "\n";
    }
}

