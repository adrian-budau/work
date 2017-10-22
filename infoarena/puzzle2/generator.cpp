#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 2) {
        cerr << "Expecting 2 arguments: N and M\n";
        return 1;
    }

    Seed::logging = false;
    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);

    if (arguments.size() > 2)
        reSeed(stoul(arguments[2]));

    vector<int> values(N * M);
    iota(values.begin(), values.end(), 1);

    vector< vector<int> > matrix(N, vector<int>(M));
    randomShuffle(values.begin(), values.end());
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            matrix[i][j] = values.back();
            values.pop_back();
        }
    }

    vector< pair<int, int> > edges;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            if (i + 1 < N)
                edges.emplace_back(matrix[i][j], matrix[i + 1][j]);
            if (j + 1 < M)
                edges.emplace_back(matrix[i][j], matrix[i][j + 1]);
        }

    cout << N * M << " " << edges.size() << "\n";
    for (auto &edge : shuffle(edges)) {
        if (randomElement({true, false})) {
            cout << edge.first << " " << edge.second << "\n";
        } else {
            cout << edge.second << " " << edge.first << "\n";
        }
    }
}
