#include <vector>
#include <algorithm>
#include <iostream>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int MAX_ITERATIONS;

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
    for (int i = g.edges().size(); i < edges; ++i) {
        addEdge(randomElement(g), randomElement(g));
    }
    g.Index();
    return g;
}

Graph<> randomBipartiteBiconnected(int nodes, int edges) {
    int iterations = randomInt(1, min(MAX_ITERATIONS, nodes - 3));
    auto partition = randomPartition(nodes - 3, iterations);

    partition[0] += 3;
    if (partition[0] % 2) {
        partition.push_back(1);
        --partition[0];
    }

    auto g = cycle<bool>(partition[0]);

    vector<Node<bool> > left, right;
    for (int i = 0; i < partition[0]; ++i)
        if (i % 2) {
            left.push_back(g[i]);
        } else {
            right.push_back(g[i]);
            g[i].data() = true;
        }
    for (int i = 1; i < iterations; ++i) {
        auto g2 = chain<bool>(2 + partition[i], Boolean::False);

        if (partition[i] % 2 == 0)
            g.fuseGraph(g2, {{randomElement(left), g2[0]}, {randomElement(right), g2[partition[i] + 1]}});
        else {
            vector<Node<bool>> picked;
            if (randomElement({true, false}))
                picked = randomSubsequence(left, 2);
            else
                picked = randomSubsequence(right, 2);
            g.fuseGraph(g2, {{picked[0], g2[0]}, {picked[1], g2[partition[i] + 1]}});
        }
    }

    for (int i = g.edges().size(); i < edges; ++i)
        addEdge(randomElement(left), randomElement(right));

    auto realG = Graph<>(g.size());
    for (auto &e : g.edges())
        addEdge(realG[e.from().index()], realG[e.to().index()]);

    return realG;
}

void dfs(const Node< pair<int, int> >& node, Graph<> &graph, const Node<>& fusion, Node< pair<int, int> > father = Node< pair<int, int> >()) {
    int size = node.data().first;

    Graph<> g;
    if (size <= 2) {
        g = chain(size);
    } else {
        if (size > 3 && randomElement({true, false})) { // bipartite
            g = randomBipartiteBiconnected(node.data().first, node.data().second);
        } else {
            g = randomBiconnected(node.data().first, node.data().second);
        }
    }
    g.Index();
    auto current = g[1];
    graph.fuseGraph(g, {{fusion, g[0]}});

    for (auto &x : node.edges())
        if (x.to() != father)
            dfs(x.to(), graph, current, node);
}

Graph<> connectedGraph(int nodes, int edges, int biconnected) {
    if (nodes == 1)
        return Graph<>(1);
    auto t = tree< pair<int, int> >(biconnected);

    auto eachNodes = randomPartition(nodes - 1, biconnected);
    auto eachEdges = randomPartition(edges, biconnected);

    for (int i = 0; i < biconnected; ++i)
        t[i].data() = make_pair(eachNodes[i] + 1, eachEdges[i]);
    auto g = Graph<>();
    // build the connected component
    dfs(t[0], g, g[0]);
    return g;
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);

    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 5) {
        cerr << "Expecting 5 arguments: N M NUM_COMPONENTS NUM_BICONNECTED MAX_ITERATIONS\n";
        return -1;
    }
    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int NUM_COMPONENTS = stoi(arguments[2]);
    int NUM_BICONNECTED = stoi(arguments[3]);
    MAX_ITERATIONS = stoi(arguments[4]);

    //Seed::logging = false;
    if (arguments.size() > 5)
        reSeed(stoul(arguments[5]));

    auto g = Graph<>(0);

    auto nodes = randomPartition(N, NUM_COMPONENTS);
    for (auto &x : nodes)
        M -= x - 1;
    auto edges = randomPartition(M, NUM_COMPONENTS, 0);
    for (int i = 0; i < NUM_COMPONENTS; ++i) {
        edges[i] += nodes[i] - 1;
        M += nodes[i] - 1;
    }

    auto biconnected = randomPartition(NUM_BICONNECTED, NUM_COMPONENTS);
    for (int i = 0; i < NUM_COMPONENTS; ++i)
        biconnected[i] = min(biconnected[i], max(nodes[i] - 1, 1));

    for (int i = 0; i < NUM_COMPONENTS; ++i)
        g.mergeGraph(connectedGraph(nodes[i], edges[i], biconnected[i]));

    g.Index(1);
    cout << N << " " << g.edges().size() << "\n";
    for (auto &e : shuffle(g.edges())) {
        cout << e.from().index() << " " << e.to().index() << "\n";
    }
}
