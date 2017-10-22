#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream cin("paint.in");
    ofstream cout("paint.out");

    int A, B, C, D; cin >> A >> B >> C >> D;

    if (D <= A || B <= C)
        cout << B - A + D - C << "\n";
    else
        cout << max(B, D) - min(A, C) << "\n";
}
