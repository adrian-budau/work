#include <iostream>
#include <string>
#include <vector>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char* argv[]) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "Expecting 4 arguments: V N K MAXDISTANCE\n";
        return 1;
    }

    int64_t V = stoll(arguments[0]);
    int N = stoi(arguments[1]);
    int K = stoi(arguments[2]);
    int MAXDISTANCE = stoi(arguments[3]);

    Seed::logging = false;
    if (arguments.size() > 4) {
        reSeed(stoul(arguments[4]));
    }

    cout << V << " " << N << " " << K << "\n";

    int64_t start = randomInt64(1, max(V - MAXDISTANCE, 1LL));
    int64_t end = min(start + MAXDISTANCE, V);

    vector<int64_t> already;
    if (K >= 2) {
        already.push_back(start);
        already.push_back(end);
        K -= 2;
        ++start;
        --end;
    }

    auto A = randomSample(K, start, end);
    already.insert(already.end(), A.begin(), A.end());
    sort(already.begin(), already.end());
    for (auto &x : already)
        cout << x << " ";
    cout << "\n";
}
