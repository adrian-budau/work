#include <iostream>
#include <tuple>
#include <vector>

#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 6) {
        cerr << "Expecting 6 arguments: N M K MAXSTEP MINLENGTH MAXV\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int K = stoi(arguments[2]);
    int MAXSTEP = stoi(arguments[3]);
    int MINLENGTH = stoi(arguments[4]);
    int MAXV = stoi(arguments[5]);
    Seed::logging = false;
    if (arguments.size() > 6)
        reSeed(stoul(arguments[6]));

    vector<tuple<int, int, int, int>> queries;
    for (int i = 0; i < M; ++i) {
        int STEP = randomInt(1, N - 1);
        if (i < K)
            STEP = randomInt(1, min(MAXSTEP, N - 1));
        int LENGTH = randomInt(max(MINLENGTH - 1, STEP), N - 1);
        int X = randomInt(1, N - LENGTH);
        int Y = X + LENGTH;
        int V = randomInt(1, MAXV);
        queries.emplace_back(X, Y, STEP, V);
    }

    ios::sync_with_stdio(false);
    cout << N << " " << M << "\n";
    for (auto &t : shuffle(queries)) {
        int X, Y, STEP, WEIGHT; tie(X, Y, STEP, WEIGHT) = t;
        cout << X << " " << Y << " " << STEP << " " << WEIGHT << "\n";
    }
}
