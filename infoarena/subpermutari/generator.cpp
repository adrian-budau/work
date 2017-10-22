#include <iostream>
#include <fstream>
#include <vector>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "Usage: N K A B\n";
        return 1;
    }

    Seed::logging = false;
    int N = stoi(arguments[0]);
    int K = stoi(arguments[1]);
    int A = stoi(arguments[2]);
    int B = stoi(arguments[3]);

    if (arguments.size() > 4)
        reSeed(stoul(arguments[4]));

    vector<int> perm(N);
    for (int i = 0; i < N; ++i)
        perm[i] = i + 1;
    randomShuffle(perm.begin(), perm.end());

    for (int i = 0; i < K; ++i) {
        int length = randomInt(A, B);
        int start = randomInt(0, N - length);
        sort(perm.begin() + start, perm.begin() + start + length);
        if (randomElement({true, false}))
            reverse(perm.begin() + start, perm.begin() + start + length);

        int start1 = randomInt(0, N - length);
        int start2 = randomInt(0, N - length);

        vector<int> have;
        for (int j = start1; j < start1 + length; ++j)
            have.push_back(perm[j]);
        vector<int> sorted = have;
        sort(sorted.begin(), sorted.end());

        vector<int> how(length);
        for (int j = 0; j < int(have.size()); ++j)
            how[lower_bound(sorted.begin(), sorted.end(), have[j]) - sorted.begin()] = j;

        have.clear();
        for (int j = start2; j < start2 + length; ++j)
            have.push_back(perm[j]);
        sorted = have;
        sort(sorted.begin(), sorted.end());

        vector<int> put(length);
        for (auto &x : have)
            put[how[lower_bound(sorted.begin(), sorted.end(), x) - sorted.begin()]] = x;
        for (int j = start2; j < start2 + length; ++j)
            perm[j] = put[j - start2];
    }

    cout << N << "\n";
    for (auto &x : perm)
        cout << x << " ";
    cout << "\n";
}
