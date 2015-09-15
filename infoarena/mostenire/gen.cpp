#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    Seed::logging = false;
    // N(sons) M(nephews) K(money) [X(optional special distributed sons) Y(optional special distributed nephews)] [seed]
    if (arguments.size() < 3 || arguments.size() > 6) {
        cerr << "USAGE: sons nephews money [special_sons special_newphews] [seed]\n";
        return -1;
    }

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int64_t K = stoll(arguments[2]);
    int X = 0, Y = 0;
    if (arguments.size() > 4) {
        X = stoi(arguments[3]);
        Y = stoi(arguments[4]);
    }

    if (arguments.size() % 2 == 0) {
        reSeed(stoul(arguments.back()));
    }

    auto first = X == 0 ? vector<int>() : randomPartition(Y, X, 1);
    auto second = randomPartition(M - Y, N - X, 1);

    auto arr(first); arr.insert(arr.end(), second.begin(), second.end());
    sort(arr.begin(), arr.end());

    cout << N << " " << K << "\n";
    for (int i = 0; i < N; ++i)
        cout << arr[i] << " ";
    cout << "\n";
}
