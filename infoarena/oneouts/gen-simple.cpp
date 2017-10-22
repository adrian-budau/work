#include <inputGenerator.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    Seed::logging = false;
    if (arguments.size() < 4) {
        cerr << "USAGE: N K MAXA MAXB\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int K = stoi(arguments[1]);
    int MAXA = stoi(arguments[2]);
    int MAXB = stoi(arguments[3]);
    if (arguments.size() > 4)
        reSeed(stoul(arguments[4]));

    cout << N << " " << K << "\n";
    for (int i = 0; i < N; ++i)
        cout << randomInt(1, MAXA) << " ";
    cout << "\n";
    for (int i = 0; i < N; ++i)
        cout << randomInt(1, MAXB) << " ";
    cout << "\n";
}

