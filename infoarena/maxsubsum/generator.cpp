#include <iostream>
#include <fstream>
#include <algorithm>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

vector<int> generate(int N, int MAXV, int MINV) {
    int number = MAXV;
    auto V = randomPartition(MAXV, N - 1, MINV);
    for (auto &x : V)
        x = -x;
    V.push_back(number);
    return shuffle(V);
}

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 6) {
        cerr << "Usage: N M MAXV1 MINV1 MAXV2 MINV2\n";
        return 1;
    }

    Seed::logging = false;
    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int MAXV1 = stoi(arguments[2]);
    int MINV1 = stoi(arguments[3]);
    int MAXV2 = stoi(arguments[4]);
    int MINV2 = stoi(arguments[5]);

    if (arguments.size() > 6)
        reSeed(stoul(arguments[6]));
    auto A = generate(N, MAXV1, MINV1);
    auto B = generate(M, MAXV2, MINV2);

    cout << N << " " << M << "\n";
    for (auto &x : A)
        cout << x << " ";
    cout << "\n";
    for (auto &x : B)
        cout << x << " ";
    cout << "\n";
}
