#include <iostream>
#include <fstream>
#include <inputGenerator.hpp>
#include <algorithm>

using namespace std;
using namespace inputGenerator;

void generateTest(int N, int MAXV, int K) {
    int totalDiff = randomInt(MAXV / 2, MAXV);
    auto diffs = randomPartition(totalDiff, N - 1, 0);
    auto nextDiffs = diffs;
    auto samp = randomSample(K, 0, N - 2);
    for (int i = 0; i < N - 1; ++i)
        nextDiffs[i] = randomInt(0, nextDiffs[i]);
    for (auto &x : samp)
        nextDiffs[x] = diffs[x];

    if (randomElement({true, false})) { // force to be solution
        // find the first that is not equal and the last
        int first_eq = -1, last_eq = -1;
        for (int i = 0; i < N - 1; ++i)
            if (diffs[i] == nextDiffs[i]) {
                last_eq = i;
                if (first_eq == -1)
                    first_eq = i;
            }
        for (int i = first_eq; i < last_eq; ++i) {
            if (diffs[i] == nextDiffs[i])
                continue;
            int sum = 0;
            int j;
            for (j = i; j < last_eq && diffs[j] != nextDiffs[j]; ++j)
                sum += diffs[j] - nextDiffs[j];
            if (sum % 2 == 1) { // just pick a random one
                nextDiffs[randomElement({i, j - 1})]++;
            }
            i = j;
        }
    }

    // now pick the begin and end for each
    int sumBegin = 0, sumEnd = 0;
    for (int i = 0; i < N - 1; ++i) {
        sumBegin += diffs[i];
        sumEnd += nextDiffs[i];
    }

    int firstBegin = randomInt(0, MAXV - sumBegin), firstEnd = randomInt(0, MAXV - sumEnd);
    cout << N << "\n";
    cout << firstBegin << " ";
    for (int i = 0; i < N - 1; ++i) {
        firstBegin += diffs[i];
        cout << firstBegin << " ";
    }
    cout << "\n";
    cout << firstEnd << " ";
    for (int i = 0; i < N - 1; ++i) {
        firstEnd += nextDiffs[i];
        cout << firstEnd << " ";
    }
    cout << "\n";
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);

    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "USAGE: T MAXN MAXV MAXK\n";
        return 1;
    }

    int T = stoi(arguments[0]);
    int MAXN = stoi(arguments[1]);
    int MAXV = stoi(arguments[2]);
    int MAXK = stoi(arguments[3]);
    Seed::logging = false;
    if (arguments.size() > 4)
        reSeed(stoul(arguments[4]));

    cout << T << "\n";
    while (T--) {
        int N = randomInt(2, MAXN);
        int K = randomInt(1, min(N - 1, MAXK));
        if (T == 0) {
            N = MAXN;
            K = min(N - 1, MAXK);
        }
        generateTest(N, MAXV, K);
    }
}
