#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <numeric>
#include <inputGenerator.hpp>

using namespace inputGenerator;
using namespace std;

vector<int> complete(vector<int> V, int minV, int maxV, int N) {
    auto numbers = randomSample(N, minV, maxV);

    set<int> taken;
    for (auto &x : V)
        taken.insert(x);
    vector<int> answer(N);

    vector<int> positions(N);
    iota(positions.begin(), positions.end(), 0);
    randomShuffle(positions.begin(), positions.end());

    int M = V.size();
    vector<bool> marked(N, false);
    for (int i = 0; i < M; ++i)
        marked[positions[i]] = true;

    int k = 0;
    int p = 0;
    for (int i = 0; i < N; ++i)
        if (marked[i])
            answer[i] = V[k++];
        else {
            while (taken.count(numbers[p]))
                ++p;
            answer[i] = numbers[p++];
        }
    return answer;
}

int main(int argc, char**argv) {
    vector<string> arguments(argv + 1, argv + argc);
    if (arguments.size() < 4) {
        cerr << "Expecting at least two arguments N, M and MINV MAXV\n";
        return -1;
    }
    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int MINV = stoi(arguments[2]);
    int MAXV = stoi(arguments[3]);
    Seed::logging = false;
    if (arguments.size() > 4) {
        reSeed(stoul(arguments[4]));
    }

    int common = randomInt(min(N + 1, M + 1) / 2, min(N, M));
    int least = randomInt(1, common);

    vector<int> numbersA = shuffle(randomSample(common, MINV, MAXV));

    vector<int> positionsA(common), positionsB(common);
    for (int i = 0; i < common; ++i)
        positionsA[i] = positionsB[i] = i;
    randomShuffle(positionsA.begin(), positionsA.end());
    randomShuffle(positionsB.begin(), positionsB.end());

    vector<bool> marked(common, false);
    for (int i = 0; i < least; ++i)
        marked[positionsB[i]] = true;
    sort(positionsA.begin(), positionsA.begin() + least);

    int k = 0; int p = least;
    vector<int> numbersB(common);
    for (int i = 0; i < common; ++i)
        if (marked[i])
            numbersB[i] = numbersA[positionsA[k++]];
        else
            numbersB[i] = numbersA[positionsA[p++]];

    cerr << common << " " << least << "\n";
    numbersA = complete(numbersA, MINV, MAXV, N);
    numbersB = complete(numbersB, MINV, MAXV, M);

    ios::sync_with_stdio(false);
    cout << N << " " << M << "\n";
    for (int i = 0; i < N; ++i)
        cout << numbersA[i] << " ";
    cout << "\n";
    for (int i = 0; i < M; ++i)
        cout << numbersB[i] << " ";
    cout << "\n";
}
