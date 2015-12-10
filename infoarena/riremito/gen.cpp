#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <inputGenerator.hpp>
#include <set>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "USAGE: N MAXEDGE K prob [seed]\n";
        return -1;
    }

    int N = stoi(arguments[0]);
    int MAXEDGE = stoi(arguments[1]);
    int K = stoi(arguments[2]);
    double prob = stod(arguments[3]);

    Seed::logging = false;
    if (arguments.size() > 4) {
        reSeed(stoul(arguments[4]));
    }

    Graph<void, int> G(2);
    addEdge(G[0], G[1], randomInt(1, MAXEDGE));

    vector<int> leafs = {0, 1};
    set<int> nonLeafs;

    for (int i = 2; i < N; ++i) {
        int x;
        if (randomDouble(0, 1) < prob) {
            do {
                int index = randomInt<int>(0, leafs.size() - 1);
                x = leafs[index];
                swap(leafs[index], leafs.back());
                leafs.pop_back();
            } while (nonLeafs.count(x));
        } else {
            x = randomInt<int>(0, G.size() - 1);
        }

        nonLeafs.emplace(x);
        leafs.push_back(G.size());

        Node<void, int> node;
        addEdge(G[x], node, randomInt(1, MAXEDGE));
        G.addNodes({node});
    }

    G.Index(1);
    cout << G.size() << "\n";
    for (auto &e : shuffle(G.edges())) {
        int x = e.from().index();
        int y = e.to().index();
        if (randomElement({true, false}))
            swap(x, y);
        cout << x << " " << y << " " << e.data() << "\n";
    }

    cout << K << "\n";
    for (auto &v : shuffle(randomSample(K, 1, N))) {
        cout << v << " ";
    }

    cerr << N - nonLeafs.size() << "\n";
    cout << "\n";
}
