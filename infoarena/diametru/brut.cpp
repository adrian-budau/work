#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream cout("diametru.out");

    cout << "499 989\n";
    for (int i = 1; i <= 291; ++i) {
        cout << i << " 493\n";
        cout << i << " 494\n";
    }
    for (int i = 292; i <= 492; ++i) {
        cout << i << " 495\n";
        cout << i << " 496\n";
    }

    cout << "493 497\n";
    cout << "494 498\n";
    cout << "497 495\n";
    cout << "498 496\n";
    cout << "497 499\n";
}
