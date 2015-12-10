#include <iostream>
#include <algorithm>
#include <vector>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argv, char **argc) {
    vector<string> arguments(argc + 1, argc + argv);

    if (arguments.size() < 4) {
        cerr << "Expecting at least 4 arguments, N, M and MAXV and MAXSHOW\n";
        return -1;
    }

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int MAXV = stoi(arguments[2]);
    int MAXSHOW = stoi(arguments[3]);

    Seed::logging = false;
    if (arguments.size() == 5)
        reSeed(stoul(arguments[4]));

    auto showCount = randomPartition(M, N);

    vector< vector<pair<int, int> > > V(N);

    vector<int> A(N, 0), B(N, 0);
    if (arguments.size() < 6) {
        for (int i = 0; i < N; ++i) {
            A[i] = randomInt(0, MAXSHOW);
            B[i] = randomInt(0, MAXSHOW);
        }
    }

    for (int i = 0; i < N; ++i) {
        auto numbers = randomSample(showCount[i] * 2, 0, MAXV);

        for (int j = 0; j < showCount[i] * 2; j += 2) {
            int x = numbers[j];
            int y = numbers[j + 1];
            if (y - x >= MAXSHOW) {
                int length = randomInt(1, MAXSHOW);
                x = randomInt(x, y - length);
                y = x + length;
            }

            V[i].emplace_back(x, y);
        }
    }

    cout << N << " " << M << "\n";
    for (int i = 0; i < N; ++i)
        cout << A[i] << " ";
    cout << "\n";
    for (int i = 0; i < N; ++i)
        cout << B[i] << " ";
    cout << "\n";

    vector<pair<int, pair<int, int> > > shows;
    for (int i = 0; i < N; ++i)
        for (auto &x : V[i])
            shows.emplace_back(i, x);

    for (auto &p : shuffle(shows)) {
        cout << p.first + 1 << " " << p.second.first << " " << p.second.second << "\n";
    }
}
