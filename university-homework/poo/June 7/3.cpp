#include <iostream>

#include "3.h"

int main() {
    SMatrix A;
    A[0][0] = 1;
    A[0][1] = 4;
    A[1][0] = 2;
    A[1][1] = 6;

    SMatrix B = A + A;
    SMatrix C = A * B;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j)
            cout << A[i][j] << " ";
        cout << "\n";
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j)
            cout << B[i][j] << " ";
        cout << "\n";
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j)
            cout << C[i][j] << " ";
        cout << "\n";
    }

    cout << C.determinant() << "\n";
}
