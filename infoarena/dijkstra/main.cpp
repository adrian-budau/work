#include <iostream>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

int main() {
    ifstream cin("dijkstra.in");
    ofstream cout("dijkstra.out");

    int N, M; cin >> N >> M;

    vector< vector< pair<int, int> > > E(N);
    for (int i = 0; i < M; ++i) {
        int x, y, z; cin >> x >> y >> z;
        --x; --y;
        E[x].push_back({y, z});
    }

    queue<int> Q;
    vector<int> inQueue(N, 0);
    vector<int> dis(N, 0x3f3f3f3f);
    vector<int> p(N, -1);
    dis[0] = 0;
    p[0] = 0;
    Q.push(0);

    while (!Q.empty()) {
        int x = Q.front(); Q.pop();
        inQueue[x] = 0;

        if (inQueue[p[x]] or inQueue[p[p[x]]])
            continue;
        for (auto &y : E[x])
            if (dis[x] + y.second < dis[y.first]) {
                dis[y.first] = dis[x] + y.second;
                p[y.first] = x;
                if (not inQueue[y.first]) {
                    inQueue[y.first];
                    Q.push(y.first);
                }
            }
    }

    for (int i = 1; i < N; ++i) {
        if (dis[i] == 0x3f3f3f3f)
            dis[i] = 0;
        cout << dis[i] << " ";
    }
    cout << "\n";
}

