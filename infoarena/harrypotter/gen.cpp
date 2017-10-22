#include <iostream>
#include <fstream>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

pair< vector<int>, vector<int> > generate_pair(int N, int K) {
    vector<int> A, B, common;
    for (int i = 0; i < N; ++i)
        common.push_back(i + 1);
    randomShuffle(common.begin(), common.end());

    // first K are what we need, now lets pick positions
    for (int i = 0; i < K; ++i) {
        A.push_back(1);
        B.push_back(1);
    }
    for (int i = K; i < N; ++i) {
        A.push_back(0);
        B.push_back(0);
    }
    randomShuffle(A.begin(), A.end());
    randomShuffle(B.begin(), B.end());
    auto commonB = common;
    randomShuffle(commonB.begin() + K, commonB.end());
    for (int i = 0; i < N; ++i) {
        if (A[i] == 0) {
            A[i] = common.back();
            common.pop_back();
        } else {
            A[i] = common.front();
            common.erase(common.begin());
        }
        if (B[i] == 0) {
            B[i] = commonB.back();
            commonB.pop_back();
        } else {
            B[i] = commonB.front();
            commonB.erase(commonB.begin());
        }
    }
    return make_pair(A, B);
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);

    vector<string> arguments(argv + 1, argv + argc);
    if (arguments.size() < 3) {
        cerr << "USAGE: N K1(top-bottom common) K2(left-right common)\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int K1 = stoi(arguments[1]);
    int K2 = stoi(arguments[2]);

    vector<int> top, bottom, left, right;
    tie(top, bottom) = generate_pair(N, K1);
    tie(left, right) = generate_pair(N, K2);
    reverse(bottom.begin(), bottom.end());
    reverse(left.begin(), left.end());

    cout << N << "\n";
    for (int i = 0; i < N; ++i)
        cout << top[i] << " ";
    cout << "\n";
    for (int i = 0; i < N; ++i)
        cout << right[i] << " ";
    cout << "\n";
    for (int i = 0; i < N; ++i)
        cout << bottom[i] << " ";
    cout << "\n";
    for (int i = 0; i < N; ++i)
        cout << left[i] << " ";
    cout << "\n";
}
