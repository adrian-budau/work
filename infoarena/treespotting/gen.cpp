#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

bool getChain(const Node<void, bool>& now, const Node<void, bool>& to, vector<Node<void, bool>>& chain, const Node<void, bool>& from = Node<void, bool>()) {
    chain.push_back(now);
    if (now == to)
        return true;

    for (auto &n : now.edges())
        if (n.to() != from)
            if (getChain(n.to(), to, chain, now))
                return true;

    chain.pop_back();

    return false;
}

void dfsAddBadEdge(const Node<void, bool> &node, vector<bool>& used, vector<int>& upEdges, vector<Node<void, bool>>& stack, const Node<void, bool>& from = Node<void, bool>()) {
    used[node.index()] = true;
    stack.push_back(node);
    for (int i = 0; i < upEdges[node.index()]; ++i)
        addEdge(randomElement(stack), node, true);

    for (auto &n : node.edges())
        if (n.to() != from && used[n.to().index()] == false)
            dfsAddBadEdge(n.to(), used, upEdges, stack, node);

    stack.pop_back();
}

void addBadEdges(Graph<void, bool>& graph, const Node<void, bool>& from, const Node<void, bool>& to, int many) {
    int direct = randomInt(0, many); // edges that are right on the chain

    vector<Node<void, bool>> chain;
    getChain(from, to, chain);

    for (int i = 0; i < direct; ++i)
        addEdge(randomElement(chain), randomElement(chain), true);

    vector<bool> used(graph.size(), false);
    for (auto &node : chain)
        used[node.index()] = true;

    vector<int> upEdges = randomPartition(many - direct + graph.size(), graph.size());
    for (auto &x: upEdges)
        x--;

    vector<Node<void, bool>> stack;
    for (auto &node : chain)
        dfsAddBadEdge(node, used, upEdges, stack);
}

int main(int argc, char* argv[]) {
    vector<string> arguments(argv + 1, argv + argc);

    int N, M, K;
    if (arguments.size() < 3) {
        cerr << "Expecting 3 arguments\n";
        return 1;
    }

    N = stoi(arguments[0]);
    M = stoi(arguments[1]);
    K = stoi(arguments[2]);

    Seed::logging = false;
    if (arguments.size() > 3) {
        reSeed(stoul(arguments[3]));
    }

    auto simpleGraph = tree<void, bool>(K);

    auto distribution = randomPartition(N - K + (2 * K - 1), 2 * K - 1);
    for (auto &x : distribution)
        --x;

    auto edges = simpleGraph.edges();
    vector<int> nodeDistribution(distribution.begin(), distribution.begin() + K);
    for (auto &x : nodeDistribution)
        ++x;

    vector<int> edgeDistribution(distribution.begin() + K, distribution.end());

    // First expand the edges, they don't break the graph pointers
    int extra = M - N + 1;
    vector<int> badEdgeDistribution = randomPartition(extra + K - 1, K - 1);
    for (auto &x : badEdgeDistribution)
        --x;

    int copyEdges = 0;
    for (int i = 0; i < K - 1; ++i) { // generate a random graph and put those bad edges here
        if (edgeDistribution[i] == 0) {
            copyEdges += badEdgeDistribution[i];
            continue;
        }

       // make a wide tree for which we know 2 nodes at a enough distance from each other
        Graph<void, bool> partialTree;
        if (edgeDistribution[i] > 1)
            partialTree = wideTree<void, bool>(edgeDistribution[i], max(2, edgeDistribution[i] / 2), Boolean::False);
        else
            partialTree = tree<void, bool>(edgeDistribution[i]);

        if (partialTree.size() > 1) {
            addBadEdges(partialTree, partialTree[0], partialTree[1], badEdgeDistribution[i]);
            simpleGraph.expandEdge(edges[i], partialTree, partialTree[0], partialTree[1]);
        } else {
            addBadEdges(partialTree, partialTree[0], partialTree[0], badEdgeDistribution[i]);
            simpleGraph.expandEdge(edges[i], partialTree, partialTree[0], partialTree[0]);
        }
    }

    for (int i = 0; i < K; ++i) {
        auto partialTree = tree<void, bool>(nodeDistribution[i]);
        vector<Node<void, bool>> goodNodes;
        for (auto &node : partialTree)
            if (node.edges().size() <= 1)
                goodNodes.push_back(node);
        simpleGraph.expandNode(simpleGraph[i], partialTree, goodNodes);
    }

    edges = simpleGraph.edges();
    for (int i = 0; i < copyEdges; ++i) {
        auto edge = randomElement(edges);
        addEdge(edge.from(), edge.to(), true);
    }

    cout << N << " " << M << "\n";
    //simpleGraph.Index(1);
    auto allEdges = shuffle(simpleGraph.edges());
    for (auto &edge : allEdges)
        if (edge.data() == false) {
            int x = edge.from().index();
            int y = edge.to().index();
            if (randomElement({true, false}))
                swap(x, y);
            ++x; ++y;
            cout << x << " " << y << "\n";
        }
    for (auto &edge : allEdges)
        if (edge.data() == true) {
            int x = edge.from().index();
            int y = edge.to().index();
            if (randomElement({true, false}))
                swap(x, y);
            ++x; ++y;
            cout << x << " " << y << "\n";
        }
}
