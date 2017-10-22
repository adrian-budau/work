#include <iostream>
#include <cassert>

#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 5) {
        cerr << "Expecting 5 arguments: N K MINV MAXV LIS\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int K = stoi(arguments[1]);
    int MINV = stoi(arguments[2]);
    int MAXV = stoi(arguments[3]);
    int LIS = stoi(arguments[4]);

    Seed::logging = false;
    if (arguments.size() > 5) {
        reSeed(stoul(arguments[5]));
    }

    vector<int> values(MAXV - MINV + 1);
    for (int i = MINV; i <= MAXV; ++i)
        values[i - MINV] = i;

    vector<int> A = randomSubsequence(values, N);
    randomShuffle(A.begin(), A.end());

    set<int> left;
    for (int i = 0; i < N + K; ++i)
        left.emplace(i + 1);

    for (auto &x : A)
        left.erase(x);
    vector<int> B(left.begin(), left.end());
    randomShuffle(B.begin(), B.end());

    auto lis_position = randomSample(LIS, 0, N - 1);
    sort(lis_position.begin(), lis_position.end());

    values.clear();
    for (auto &x : lis_position)
        values.push_back(A[x]);
    sort(values.begin(), values.end());
    while (lis_position.size()) {
        A[lis_position.back()] = values.back();
        values.pop_back();
        lis_position.pop_back();
    }

    cout << N << " " << K << "\n";
    for (int i = 0; i < N; ++i)
        cout << A[i] << " ";
    cout << "\n";
    for (int i = 0; i < K; ++i)
        cout << B[i] << " ";
    cout << "\n";
}
