#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

int main() {
    int N, M, Q;
    while (cin >> N >> M >> Q) {
        if (N == 0 and M == 0 and Q == 0)
            break;

        int S = 0;
        vector< vector< pair<int, int> > > E(N + M + 1);
        for (int i = 0; i < Q; ++i) {
            string semn;
            int A, B, C;
            cin >> A >> B >> semn >> C;

            if (semn == "<=") // a[A] <= -b[B] + C
                E[B + N].push_back({A, C});
            else // a[A] - C >= -b[B]
                E[A].push_back({B + N, -C});
        }

        for (int i = 1; i <= N + M; ++i)
            E[S].push_back({i, 0});

        vector<int> distance(N + M + 1, numeric_limits<int>::max());
        distance[S] = 0;

        bool ok = true;
        for (int i = 1; i <= N + M + 2 and ok; ++i)
            for (int j = 0; j <= N + M and ok; ++j)
                for (auto &next : E[j])
                    if (distance[j] + next.second < distance[next.first]) {
                        if (i == N + M + 2) {
                            cout << "Impossible\n";
                            ok = false;
                            break;
                        }
                        distance[next.first] = distance[j] + next.second;
                    }
        if (ok)
            cout << "Possible\n";
    }
}
