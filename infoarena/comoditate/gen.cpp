#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <unordered_set>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int MAX_ITERATIONS = 50;

template<class T = void>
Graph<T> cycle(int size) {
    auto g = chain<T>(size, Boolean::False);
    addEdge(g[0], g[size - 1]);
    return g;
}

Graph<> randomBiconnected(int nodes, int edges) {
    int iterations = randomInt(1, min(MAX_ITERATIONS, nodes - 2));

    auto partition = randomPartition(nodes - 2, iterations);
    partition[0] += 2;
    auto g = cycle(partition[0]);

    for (int i = 1; i < iterations; ++i) {
        auto g2 = chain(2 + partition[i], Boolean::False);
        auto picked = randomSample(2, 0, g.size() - 1);
        g.fuseGraph(g2, {{g[picked[0]], g2[0]}, {g[picked[1]], g2[partition[i] + 1]}});
    }

    int extraEdges = max<int>(edges - g.edges().size(), 0);
    auto edgeIndexes = shuffle(randomSample(edges, 0LL, 1LL * nodes * (nodes - 1) / 2 - 1));
    unordered_set<int64_t> existing;

    for (auto &e : g.edges()) {
        int x = e.from().index();
        int y = e.to().index();
        if (x > y)
            swap(x, y);
        int64_t value = x * nodes - 1LL * x * (x + 1) / 2;
        existing.insert(value);
    }
    for (int i = 0; i < int(edgeIndexes.size());)
        if (existing.count(edgeIndexes[i])) {
            swap(edgeIndexes.back(), edgeIndexes[i]);
            edgeIndexes.pop_back();
        } else {
            ++i;
        }

    edgeIndexes.resize(extraEdges);
    sort(edgeIndexes.begin(), edgeIndexes.end());
    int x = 0;
    int64_t now = 0;
    int64_t next = nodes - 1;
    for (auto &v : edgeIndexes) {
        while (v >= next) {
            now = next;
            ++x;
            next += nodes - x - 1;
        }
        int y = v - now + x + 1;
        addEdge(g[x], g[y]);
    }
    g.Index();
    return g;
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);

    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "Expecting 4 arguments: T MAXM UPPER_BOUND LOWER_LOG";
        return -1;
    }

    int T = stoi(arguments[0]);
    int MAXM = stoi(arguments[1]);
    int UPPER_BOUND = stoi(arguments[2]);
    double LOWER_LOG = stod(arguments[3]);
    Seed::logging = false;
    if (arguments.size() > 4)
        reSeed(stoul(arguments[4]));

    vector<int> M(T, 3);
    vector<int> pos(T);
    for (int i = 0; i < T; ++i)
        pos[i] = i;
    MAXM -= 3 * T;
    while (MAXM > 0) {
        int x = randomInt(0, int(pos.size()) - 1);
        int v = pos[x];
        if (++M[v] == UPPER_BOUND) {
            swap(pos[x], pos.back());
            pos.pop_back();
        }
        --MAXM;
    }

    cout << T << "\n";
    for (int i = 0; i < T; ++i) {
        double ratio = randomDouble(LOWER_LOG, 1.0);
        int N = pow(M[i], ratio);

        if (N > M[i])
            N = M[i];
        while (1LL * N * (N - 1) / 2 < M[i])
            ++N;
        auto g = randomBiconnected(N, M[i]);
        g.Index(1);
        cout << g.size() << " " << g.edges().size() << "\n";
        for (auto &e : shuffle(g.edges())) {
            int x = e.from().index();
            int y = e.to().index();
            if (randomElement({true, false}))
                swap(x, y);
            cout << x << " " << y << "\n";
        }
        cout << "\n";
        cerr << ".";
    }
}
