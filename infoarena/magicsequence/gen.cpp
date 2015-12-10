#include <inputGenerator.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace inputGenerator;

int main(int argv, char **argc) {
    vector<string> arguments(argc + 1, argc + argv);
    if (arguments.size() < 3) {
        cerr << "Expecting at least 3 arguments T, MAXN and MAXV" << "\n";
        return -1;
    }

    int T = stoi(arguments[0]);
    int MAXN = stoi(arguments[1]);
    int MAXV = stoi(arguments[2]);
    Seed::logging = false;
    if (arguments.size() >= 4)
        reSeed(stoi(arguments[3]));

    vector< vector<int> > V(T);
    for (int i = 0; i < T; ++i) {
        int N = randomInt(1, MAXN);
        if (i == 0 || T == 20)
            N = MAXN;
        V[i] = shuffle(randomSample(N, 1, MAXV));
    }

    if (T == 5)
        V[2] = vector<int>(1, randomInt(1, MAXV));

    randomShuffle(V.begin(), V.end());
    cout << T << "\n";
    for (auto &v : V) {
        cout << v.size() << "\n";
        for (auto &x : v)
            cout << x << " ";
        cout << "\n";
    }
}
