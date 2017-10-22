#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <unordered_set>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);

    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "Expecting 4 arguments: T MINN MAXN K";
        return -1;
    }

    int T = stoi(arguments[0]);
    int MINN = stoi(arguments[1]);
    int MAXN = stoi(arguments[2]);
    int K = stoi(arguments[3]);
    Seed::logging = false;
    if (arguments.size() > 4)
        reSeed(stoul(arguments[4]));

    cout << T << "\n";
    for (int i = 0; i < T; ++i) {
        int N = randomInt(MINN, MAXN);
        int MINNK = 1;
        while (MINNK * (MINNK - 1) / 2 < K)
            ++MINNK;
        int NK = randomInt(MINNK, min(2 * K, N));

        auto g = undirectedGraph(NK, K);
        g.Index(0);
        auto index = randomSample(NK, 1, N);
        cout << N << " " << K << "\n";
        for (auto &e : shuffle(g.edges())) {
            int x = index[e.from().index()];
            int y = index[e.to().index()];
            if (randomElement({true, false}))
                swap(x, y);
            cout << x << " " << y << "\n";
        }
    }
}
