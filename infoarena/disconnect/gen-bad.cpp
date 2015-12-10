#include <inputGenerator.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace inputGenerator;

int main(int argv, char **argc) {
    vector<string> arguments(argc + 1, argc + argv);

    if (arguments.size() < 2) {
        cerr << "You need at least two arguments N and M\n";
        return -1;
    }

    Seed::logging = false;
    if (arguments.size() >= 3)
        reSeed(stoul(arguments[2]));

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    auto T = tree(N);
    T.Index(1);

    vector< pair<int, int> > edges;
    for (auto &edge : shuffle(T.edges())) {
        int x = edge.from().index();
        int y = edge.to().index();
        if (randomElement({true, false}))
            swap(x, y);
        edges.emplace_back(x, y);
    }

    int deletes = randomInt(0, min(M, N - 1));

    vector< pair<int, int> > deleted = shuffle(randomSubsequence(edges, deletes));

    vector<int> ops(M, 2);
    for (int i = 0; i < deletes; ++i)
        ops[i] = 1;
    randomShuffle(ops.begin(), ops.end());

    cout << N << " " << M << "\n";
    for (auto &e : edges)
        cout << e.first << " " << e.second << "\n";
    for (auto &t : ops) {
        if (t == 1) {
            int x, y;
            tie(x, y) = deleted.back();
            deleted.pop_back();
            if (randomElement({true, false}))
                swap(x, y);
            cout << 1 << " " << x << " " << y << "\n";
        } else
            cout << 2 << " " << randomInt(1, N) << " " << randomInt(1, N) << "\n";
    }
}
