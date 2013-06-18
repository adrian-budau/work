#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

int backtracking(int state, map<int, int> &mex) {
    if (state == 0)
        return (mex[state] = 0);

    if (mex.count(state))
        return mex[state];

    vector<int> used(30, 0);
    for (int i = 0; (1 << i) <= state; ++i)
        if ((1 << i) & state) {
            int newState = state;
            for (int j = i; j <= 30; j += i + 1)
                if ((1 << j) & newState)
                    newState ^= (1 << j);
            used[backtracking(newState, mex)] = 1;
        }

    for (int i = 0; i < 30; ++i)
        if (not used[i])
            return (mex[state] = i);
    return 0;
}

int precalc(int N) {

    map<int, int> mex;

    int state = (1 << N) - 1;
    return backtracking(state, mex);
}

int main() {
    int N; cin >> N;

    ofstream data("data.txt");

    data << "{";
    for (int i = 0; i <= N; ++i) {
        data << precalc(i);
        if (i < N)
            data << ", ";
    }

    data << "}\n";
}
