#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int abs(int x) {
    if (x < 0)
        return -x;
    return x;
}

int main() {
    ios::sync_with_stdio(false);

    int X1, Y1, X2, Y2; cin >> X1 >> Y1 >> X2 >> Y2;
    int DX = abs(X1 - X2);
    int DY = abs(Y1 - Y2);

    if (DX < DY)
        swap(DX, DY);
    // DX == DY -> 2 * DX
    // DX - DY is even  we have to do 1 move for each DX anyway so -> 2 * DX
    // DX - DY is odd, same deal almost
    if ((DX - DY) % 2)
        cout << 2 * DX - 1 << "\n";
    else
        cout << 2 * DX << "\n";
}
