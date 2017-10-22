#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("symmetricgraph2.in");
    ofstream cout("symmetricgraph2.out");

    int N, M; assert(cin >> N >> M);
    assert(1 <= N && N <= 100 * 1000);
    assert(1 <= M && M <= 200 * 1000);
    vector< map<int, int64_t> > cap(N);
    for (int i = 0; i < M; ++i) {
        int x, y, c; assert(cin >> x >> y >> c);
        assert(1 <= x && x <= N && 1 <= y && y <= N && 1 <= c && c <= 1000 * 1000 * 1000);
        --x; --y;
        cap[x][y] += c;
        cap[y][x] += c;
    }

    set<int> degree2;
    for (int i = 1; i < N - 1; ++i)
        if (cap[i].size() == 2)
            degree2.insert(i);

    while (degree2.size()) {
        int x = *degree2.begin();
        degree2.erase(degree2.begin());
        int y = cap[x].begin()->first;
        int64_t yc = cap[x].begin()->second;
        int z = cap[x].rbegin()->first;
        int64_t zc = cap[x].rbegin()->second;

        cap[x].clear();
        degree2.erase(y);
        degree2.erase(z);

        cap[y].erase(x);
        cap[z].erase(x);
        cap[y][z] += min(yc, zc);
        cap[z][y] += min(yc, zc);
        if (cap[y].size() == 2 && y != 0 && y != N - 1)
            degree2.insert(y);
        if (cap[z].size() == 2 && z != 0 && z != N - 1)
            degree2.insert(z);
    }

    cout << cap[0][N - 1] << "\n";
}
