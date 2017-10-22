#include <iostream>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

double PROB;
int MAXMIN;
int MAXC;

int dfs(Node<void, int> node, Node<void, int> father) {
    int64_t sum = 0;
    for (auto &e : node.edges()) {
        auto x = e.to();
        if (x != father) {
            if (x.index() != node.index())
                sum += dfs(x, node);
            else
                sum += e.data();
        }
    }
    if (node.edgesTo(father).size() == 0)
        return 0;
    if (randomDouble(0, 1) <= PROB) {
        int cost = randomInt(1, MAXMIN);
        node.edgesTo(father)[0].data() = cost;
        return min<int64_t>(cost, sum);
    }

    // otherwise we wanna raise it at least
    int cost = randomInt(min<int64_t>(MAXC, sum), int64_t(MAXC));
    node.edgesTo(father)[0].data() = cost;
    return min<int64_t>(cost, sum);
}

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);
    if (arguments.size() < 5) {
        cerr << "USAGE: N K MAXC PROB MAXMIN\n";
        return 1;
    }

    Seed::logging = false;
    int N = stoi(arguments[0]);
    int K = stoi(arguments[1]);
    MAXC = stoi(arguments[2]);
    PROB = stod(arguments[3]);
    MAXMIN = stoi(arguments[4]);

    auto tree = wideTree<void, int>(N, K, Boolean::False);
    auto tree2 = tree.clone();

    for (int i = 1; i < N; ++i)
        if (tree[i].edges().size() == 1)
            addEdge(tree[i], tree2[i], randomInt(1, MAXMIN));

    dfs(tree[0], Node<void, int>());
    dfs(tree2[0], Node<void, int>());
    tree.mergeGraph(tree2);

    tree.Index({{tree[0], 1}, {tree2[0], 2 * N}}, 1);
    cout << 2 * N << " " << tree.edges().size() << "\n";
    for (auto &edge : shuffle(tree.edges())) {
        int from = edge.from().index();
        int to = edge.to().index();
        if (randomElement({true, false}))
            swap(from, to);
        cout << from << " " << to << " " << edge.data() << "\n";
    }
}
