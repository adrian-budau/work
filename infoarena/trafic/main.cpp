#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>

using namespace std;

static const int kInfinite = numeric_limits<int>::max() / 2;

vector<int> minDistance(int start, const vector< vector<int> > &dist) {
    vector<int> distance(dist.size(), kInfinite);
    distance[start] = 0;

    vector<bool> used(dist.size(), false);

    for (int i = 0; i < int(dist.size()); ++i) {
        int best = find(used.begin(), used.end(), false) - used.begin();
        for (int j = 0; j < int(dist.size()); ++j)
            if (not used[j])
                if (distance[j] < distance[best])
                    best = j;
        used[best] = true;
        for (int j = 0; j < int(dist.size()); ++j)
            distance[j] = min(distance[j], distance[best] + dist[best][j]);
    }
    return distance;
}

inline int min(int a, int b) {
    return a < b ? a : b;
}

inline int max(int a, int b) {
    return a > b ? a : b;
}

int main() {
    ifstream cin("trafic.in");
    ofstream cout("trafic.out");

    int N, M, G; cin >> N >> M >> G;

    vector< vector<int> > dist(N, vector<int>(N, kInfinite));
    vector< vector<int> > edge(N);
    for (int i = 0; i < M; ++i) {
        int x, y, z; cin >> x >> y >> z;
        dist[x - 1][y - 1] = dist[y - 1][x - 1] = z * 10;
        edge[x - 1].emplace_back(y - 1);
        edge[y - 1].emplace_back(x - 1);
    }

    int source = 0;
    int sink = N - 1;
    auto dSource = minDistance(source, dist), dSink = minDistance(sink, dist);

    int maxv = *max_element(dSource.begin(), dSource.end()) + *max_element(dSink.begin(), dSink.end());

    int step;
    for (step = 1; step < maxv; step <<= 1);

    vector< vector<int> > C(N, vector<int>(N, 0));
    vector<int> parent(N);
    queue<int> Q;
    auto bfs = [&]() {
        Q.push(source);
        fill(parent.begin(), parent.end(), -1);
        parent[source] = source;
        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            if (node == sink)
                continue;
            for (auto &i : edge[node]) {
                if (C[node][i] && parent[i] == -1) {
                    parent[i] = node;
                    Q.push(i);
                }
            }
        }
        return parent[sink] != -1;
    };

    int answer;
    for (answer = 0; step; step >>= 1) {
        answer += step;

        for (int x = 0; x < N; ++x)
            for (auto &y : edge[x]) {
                int leftX = max(-1, answer - dSource[x]), leftY = max(-1, answer - dSink[x]);
                int rightX = max(-1, answer - dSource[y]), rightY = max(-1, answer - dSink[y]);


                rightX = dist[x][y] - rightX;
                rightY = dist[x][y] - rightY;
                if (leftX > rightX)
                    leftX = rightX = 0;
                if (leftY > rightY)
                    leftY = rightY = 0;
                if (rightX <= leftY || rightY <= leftX)
                    C[x][y] = C[y][x] = 1;
                else if (min(leftX, leftY) != -1 || max(rightX, rightY) != dist[x][y] + 1)
                    C[x][y] = C[y][x] = 1;
                else
                    C[x][y] = C[y][x] = G + 1;

            }
        int flow = 0;
        while (bfs())
            for (auto &from : edge[sink])
                if (C[from][sink] && parent[from] != -1) {
                    int aux = C[from][sink];
                    for (int i = from; i != source; i = parent[i])
                        aux = min(aux, C[parent[i]][i]);
                    for (int i = from; i != source; i = parent[i]) {
                        C[parent[i]][i] -= aux;
                        C[i][parent[i]] += aux;
                    }

                    C[from][sink] -= aux;
                    C[sink][from] += aux;
                    flow += aux;
                }
        if (flow <= G)
            answer -= step;
    }

    ++answer;
    if (answer > maxv) {
        cout << "-1\n";
    } else {
        cout << answer / 10 << "." << answer % 10 << "\n";
    }
}

