#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 2) {
        cout << "Expecting: N and T\n";
        return -1;
    }

    int N = stoi(arguments[0]);
    int T = stoi(arguments[1]);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    if (arguments.size() > 2) {
        seed = stoul(arguments[2]);
    }

    default_random_engine generator(seed);
    normal_distribution<double> distribution(0.0, 1.0);

    vector<double> generated;
    for (int i = 0; i < N; ++i)
        generated.push_back(fabs(distribution(generator)));

    double total = accumulate(generated.begin(), generated.end(), 0.0);

    vector<int> real(N, 1);
    for (int i = 0; i < N; ++i)
        real[i] += (T - N) * generated[i] / total;

    int have = accumulate(real.begin(), real.end(), 0);
    real[0] += T - have;

    shuffle(real.begin(), real.end(), generator);

    uniform_int_distribution<int> pick(0, T - 1);
    int A = pick(generator), B = pick(generator);

    cout << N << " " << A << " " << B << "\n";
    for (int i = 0; i < N; ++i)
        cout << real[i] << " ";
    cout << "\n";
}
