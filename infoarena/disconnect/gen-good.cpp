#include <inputGenerator.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace inputGenerator;

int main(int argv, char **argc) {
    vector<string> arguments(argc + 1, argc + argv);

    if (arguments.size() < 3) {
        cerr << "You need at least three arguments N, M, K(iterations) and the step (from last) to begin erasing edges\n";
        return -1;
    }

    Seed::logging = false;
    if (arguments.size() >= 5)
        reSeed(stoul(arguments[4]));

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int K = stoi(arguments[2]);
    int WHEN = stoi(arguments[3]);
    auto T = Graph<>(1);
    auto how = randomPartition(N - 1, K);
    vector< vector<pair<Node<>, Node<>>> > edges(K);

    for (int i = 0; i < K; ++i) {
        vector<Node<>> possible(T.begin(), T.end());

        for (int j = 0; j < how[i]; ++j) {
            Node<> newNode;
            Node<> oldNode = randomElement(possible);
            addEdge(newNode, oldNode);
            T.addNodes({newNode});
            edges[i].emplace_back(newNode, oldNode);
        }
    }
    cout << N << " " << M << "\n";
    T.Index(1);
    for (auto &e : shuffle(T.edges())) {
        int x = e.from().index();
        int y = e.to().index();
        if (randomElement({true, false}))
            swap(x, y);
        cout << x << " " << y << "\n";
    }

    int possibleEdges = 0;
    for (int i = 0; i < K - WHEN; ++i)
        possibleEdges += edges[i].size();

    int deletes = randomInt(0, min(M, possibleEdges));

    vector<int> ops(M, 2);
    for (int i = 0; i < deletes; ++i)
        ops[i] = 1;
    randomShuffle(ops.begin(), ops.end());


    for (auto &t : ops) {
        if (t == 1) {
            int x, y;
            for (int i = K - WHEN - 1; i >= 0; --i)
                if (edges[i].size()) {
                    Node<> X, Y;
                    tie(X, Y) = edges[i].back();

                    x = X.index();
                    y = Y.index();
                    edges[i].pop_back();
                    break;
                }
            if (randomElement({true, false}))
                swap(x, y);
            cout << 1 << " " << x << " " << y << "\n";
        } else
            cout << 2 << " " << randomInt(1, N) << " " << randomInt(1, N) << "\n";
    }
}
