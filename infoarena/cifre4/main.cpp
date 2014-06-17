#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <array>
#include <algorithm>

using namespace std;

array<int, 4> mul{2, 3, 5, 7};

int main() {
    ifstream cin("cifre4.in");
    ofstream cout("cifre4.out");

    int T; cin >> T;
    while (T--) {
        int N, P; cin >> N >> P;

        vector< pair<int, int> > from(P, {-1, -1});
        queue<int> Q;
        from[0] = {0, 0};
        Q.push(0);

        while (!Q.empty()) {
            int now = Q.front();
            Q.pop();

            if (now == N)
                break;

            for (auto &value : mul) {
                int next = (now * 10 + value) % P;
                if (from[next].first != -1)
                    continue;
                from[next] = {now, value};
                Q.push(next);
            }
        }

        if (from[N].first == -1) {
            cout << "-1\n";
            continue;
        }

        string answer;
        for (int i = N; i != 0; i = from[i].first)
            answer += char(from[i].second % 10 + '0');
        reverse(answer.begin(), answer.end());
        cout << answer << "\n";
    }
}

