#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int main() {
    ofstream cout("hack.out");

    vector<string> A(50, string(50, '1'));

    for (int i = 0; i < 50; ++i)
        for (int j = 0; j < 50; ++j)
            if (i == 0) {
                if (j % 4 != 3)
                    A[i][j] = '0';
            } else if (i == 49) {
                if (j % 4 != 1)
                    A[i][j] = '0';
            } else
                if (j % 2 == 0)
                    A[i][j] = '0';

    cout << "50 50 50 1\n";
    for (int i = 0; i < 50; ++i)
        cout << A[i] << "\n";
}
