#include <iostream>

#include "2.h"

using namespace std;

int main() {
    Matrix<int> M(5, 5, 8);

    Matrix<int> P(5, 5, 5);

    Matrix<int> V = M * P;

    for (int i = 0; i < V.rows(); ++i) {
        for (int j = 0; j < V.columns(); ++j)
            cout << V[i][j] << " ";
        cout << "\n";
    }

    Matrix<int> U = M + P;
    for (int i = 0; i < U.rows(); ++i) {
        for (int j = 0; j < U.columns(); ++j)
            cout << U[i][j] << " ";
        cout << "\n";
    }
}
