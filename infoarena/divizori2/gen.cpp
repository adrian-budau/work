#include <iostream>
#include <vector>
#include <algorithm>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

/**
 * The idea is to have two proper divisors that are prime with each other
 * the easy way is to generate a small divisor by size A
 * then make the second divizor of size K * A + 1 which is B, its build of K A's and an extra node, connected to some node
 * and now we need to construct a graph with A * B nodes, we do this by cloning the second divizor (of size B) a times, and always connect the lonely node such a way to make the clone of A
 * we have a graph of size (K * A + 1) * A = K * A * A + A
 */

Graph<> extend(Graph<> start, int multiply) {
    vector<Graph<>> clones;
    for (int i = 0; i < multiply; ++i)
        clones.push_back(cloneGraph(start));

    int depth = randomInt(1, multiply);
    auto T = wideTree(multiply, depth);
    for (auto &edge : T.edges()) {
        int from = edge.from().index();
        int to = edge.to().index();
        addEdge(randomElement(clones[from]), randomElement(clones[to]));
    }

    Graph<> G = clones[0];
    for (int i = 1; i < multiply; ++i)
        G.mergeGraph(clones[i]);

    return G;
}

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 2) {
        cerr << "USAGE: A K [div1 [div2 [div3 [div4 .... ]]]]\n";
        return -1;
    }

    Seed::logging = false;
    int A = stoi(arguments[0]);
    int K = stoi(arguments[1]);
    vector<int> mul;
    for (int i = 2; i < int(arguments.size()); ++i)
        mul.push_back(stoi(arguments[i]));

    int depth = randomInt(1, A);
    auto GA = wideTree(A, depth);

    auto GB = extend(GA, K);
    Node<> node;
    addEdge(node, randomElement(GB));
    GB.addNodes({node});

    vector<Graph<>> clones;
    for (int i = 0; i < A; ++i)
        clones.emplace_back(cloneGraph(GB));

    for (auto &edge : GA.edges()) {
        int from = edge.from().index();
        int to = edge.to().index();

        addEdge(clones[from][node.index()], clones[to][node.index()]);
    }

    auto GC = clones[0];
    for (int i = 1; i < A; ++i)
        GC.mergeGraph(clones[i]);

    for (auto &x : mul)
        GC = extend(GC, x);

    GC.Index(1);
    cout << GC.size() << "\n";
    for (auto &edge : shuffle(GC.edges())) {
        int from = edge.from().index();
        int to = edge.to().index();
        if (randomElement({true, false}))
            swap(from, to);
        cout << from << " " << to << "\n";
    }
}

