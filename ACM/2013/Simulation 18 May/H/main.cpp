#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

using namespace std;

bitset<100000000> bad;

int main() {
    ofstream cout("main.py");
    int N; cin >> N;
    vector<int> A(N + 1);
    A[1] = 1;

    bad[1] = 1;
    for (int i = 2; i <= N; ++i) {
        int k;
        for (k = A[i - 1] + 1; bad[k]; ++k);
        A[i] = k;

        for (int j = 1; j <= i; ++j)
            for (int k = 1; k <= i; ++k)
                if (A[j] + A[k] - A[i] > 0)
                    bad[A[j] + A[k] - A[i]] = 1;
        for (int j = 1; j <= i; ++j)
            for (int k = 1; k <= i; ++k)
                bad[A[i] + A[j] - A[k]] = 1;
    }

    cout << "[";
    for (int i = 1; i <= N; ++i) {
        cout << A[i];
        if (i < N)
            cout << ",";
    }

    cout << "]";
    cout << "\n";
}
