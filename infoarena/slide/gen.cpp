#include <iostream>
#include <inputGenerator.hpp>
#include <algorithm>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);

    vector<string> arguments(argv + 1, argv + argc);
    if (arguments.size() < 3) {
        cerr << "USAGE: FILL EMPTY PARTS [ZERO_DIFFS [SEED]]\n";
        return 1;
    }

    int FILL = stoi(arguments[0]);
    int EMPTY = stoi(arguments[1]);
    int PARTS = stoi(arguments[2]);
    int ZERO_DIFFS = 0;
    if (arguments.size() > 3) {
        ZERO_DIFFS = stoi(arguments[3]);
        if (arguments.size() > 4) {
            reSeed(stoul(arguments[4]));
        }
    }

    auto walls = randomPartition(FILL, PARTS, 1);
    auto diffs = randomPartition(EMPTY, PARTS + 1 - ZERO_DIFFS, 0);
    vector<int> real_diffs(PARTS + 1, 1);
    for (int i = 1; i <= ZERO_DIFFS; ++i)
        real_diffs[i] = 0;
    randomShuffle(real_diffs.begin() + 1, real_diffs.end() - 1);
    for (auto &x : real_diffs)
        if (x == 1) {
            x = diffs.back();
            diffs.pop_back();
        }
    for (int i = 0; i < real_diffs[0]; ++i)
        cout << ".";
    for (int i = 0; i < PARTS; ++i) {
        for (int j = 0; j < walls[i]; ++j)
            if (i % 2)
                cout << "@";
            else
                cout << "#";
        for (int j = 0; j < real_diffs[i + 1]; ++j)
            cout << ".";
    }
    cout << "\n";
}
