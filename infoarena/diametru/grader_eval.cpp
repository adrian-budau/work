#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <queue>

using namespace std;

void result(string message, int score = 0) {
    cerr << message;
    cout << score;
    exit(0);
}

struct comp {
    bool operator()(const pair<int, int> &a, const pair<int, int> &b) {
        if (a.first != b.first)
            return a.first > b.first;
        return a.second < b.second;
    }
};

int main() {
    ifstream output("diametru.out");

    if (!output)
        result("Fisier de iesire lipsa");


    int N, M;
    if (!(output >> N >> M))
        result("Fisier de iesire incomplet");

    if (N < 1 || N > 500)
        result("Fisier de iesire corupt");

    if (M < 0 || M > N * (N - 1) / 2)
        result("Fisier de iesire corupt");

    vector< set<int> > E(N);
    for (int i = 0; i < M; ++i) {
        int x, y;
        if (!(output >> x >> y))
            result("Fisier de iesire incomplet");
        if (x < 0 || x > N || y < 0 || y > N || x == y)
            result("Fisier de iesire corupt");
        --x; --y;
        E[x].emplace(y);
        E[y].emplace(x);
    }

    int have = 0;
    for (auto &s : E)
        have += s.size();
    if (have != 2 * M)
        result("Fisier de iesire corupt");

    vector< vector<int> > dist(N, vector<int>(N, -1));
    for (int i = 0; i < N; ++i) {
        queue<int> Q;
        Q.push(i);
        dist[i][i] = 0;

        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            for (auto &next : E[node])
                if (dist[i][next] == -1) {
                    dist[i][next] = dist[i][node] + 1;
                    Q.push(next);
                }
        }
    }

    int answer = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            if (dist[i][j] == -1)
                result("Fisier de iesire corupt");
            answer = max(answer, dist[i][j]);
        }

    vector< set<pair<int, int> , comp> > left(N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i != j)
                left[i].emplace(dist[i][j], j);

    int now = 0;
    for (int iteration = 1;; ++iteration) {
        int cost, where;
        tie(cost, where) = *left[now].begin();
        left[now].erase(make_pair(cost, where));
        left[where].erase(make_pair(cost, now));
        if (cost == answer) {
            if (iteration >= 60 * 1000)
                result("OK", 100);
            else if (iteration >= 10 * 1000)
                result("Punctaj partial", 75);
            else if (iteration >= 450)
                result("Punctaj partial", 50);
            else if (iteration >= 10)
                result("Punctaj partial", 30);
            else
                result("Wrong Answer", 0);
        }
        now = where;
    }
}
