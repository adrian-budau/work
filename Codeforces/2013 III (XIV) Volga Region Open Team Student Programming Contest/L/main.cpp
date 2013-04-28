#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int N, U; cin >> N >> U;

    if (U >= N) {
        cout << "-1\n";
        return 0;
    }

    vector<int> T(N);
    for (int i = 0; i < N; ++i)
        cin >> T[i];

    sort(T.begin(), T.end(), [](int a, int b) {
        return a > b;
    });
    T.resize(N - U);

    cout << T.back() << "\n";
}
