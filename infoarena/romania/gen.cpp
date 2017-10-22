#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 3) {
        cerr << "Expecting 3 arguments: N K P\n";
        return 1;
    }

    Seed::logging = false;
    int N = stoi(arguments[0]);
    int K = stoi(arguments[1]);
    int P = stoi(arguments[2]);

    if (arguments.size() > 3)
        reSeed(stoul(arguments[3]));

    auto parts = randomPartition(K, P);
    for (int i = P + 1; i <= N; ++i)
        parts.emplace_back(0);
    randomShuffle(parts.begin(), parts.end());
    cout << N << " " << K << "\n";
    vector<int> V;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < parts[i]; ++j)
            V.emplace_back(i);
    for (auto &x : shuffle(V))
        cout << x + 1 << "\n";
}
