#include <iostream>
#include <string>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

// cycle 1 -> 2 -> 3 -> 4 -> 5 -> .... -> 2x -> 1 and edge k -> k + x + 1, it behaves exactly like a cycle of size x
Graph<> cheat(int size) {
    Graph<> G(2 * size);
    for (int i = 0; i < 2 * size; ++i)
        G[i].addEdge(G[(i + 1) % (2 * size)]);

    auto starts = randomSample(0, 2 * size - 1);
    for (auto &x : starts)
        G[x].addEdge(G[(x + size + 1) % (2 * size)]);
    return G;
}

int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

vector<int> partition(int N, int parts, int minimum) {
    vector<int> result(1, minimum);
    N -= minimum;
    for (int i = minimum + 1; int(result.size()) < parts; ++i) {
        bool good = true;
        for (auto &x : result)
            if (gcd(x, i) > 1)
                good = false;
        if (!good)
            continue;
        if (N - i < minimum * (parts - int(result.size()) - 1))
            break;
        result.emplace_back(i);
        N -= i;
    }

    for (int i = minimum + 1; int(result.size()) < parts; ++i) {
        bool good = true;
        for (auto &x : result)
            if (x == i)
                good = false;
        if (!good)
            continue;
        if (N - i < minimum * (parts - int(result.size()) - 1))
            break;
        result.emplace_back(i);
        N -= i;
    }
    while (int(result.size()) < parts) {
        result.emplace_back(minimum);
        N -= minimum;
    }

    result.back() += N;

    return result;
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);

    vector<string> arguments(argv + 1, argv + argc);
    if (arguments.size() < 6) {
        cerr << "Expecting 6 arguments: N K PARTS OUTER MINSIZE MAXVAL\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int K = stoi(arguments[1]);
    int PARTS = stoi(arguments[2]);
    int OUTER = stoi(arguments[3]);
    int MINSIZE = stoi(arguments[4]);
    int MAXVAL = stoi(arguments[5]);

    Seed::logging = false;
    if (arguments.size() > 6)
        reSeed(stoul(arguments[6]));

    auto parts = partition(N / 2, PARTS, MINSIZE);
    vector<Graph<>> components;
    for (auto &x : parts)
        components.emplace_back(cheat(x));
    randomShuffle(components.begin(), components.end());
    Graph<> G(0);
    if (N % 2)
        components.emplace_back(Graph<>(1));
    for (int i = 0; i < int(components.size()); ++i) {
        if (i >= OUTER) {
            int dragged_arcs = randomInt(1, components[i].size() * G.size());
            auto new_arcs = randomSample(dragged_arcs, 0, components[i].size() * G.size() - 1);
            for (auto &arc : new_arcs) {
                int x = arc % G.size();
                int y = arc / G.size();
                G[x].addEdge(components[i][y]);
            }
        }
        G.mergeGraph(components[i]);
    }

    G.Index(1);
    cout << N << " " << G.arcs().size() << " " << K << "\n";
    for (auto &e : shuffle(G.arcs()))
        cout << e.from().index() << " " << e.to().index() << "\n";

    for (int i = 0; i < N; ++i)
        cout << randomInt(1, MAXVAL) << " ";
    cout << "\n";
}
