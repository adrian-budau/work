#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream cout("diametru.out");

    cout << "498 661\n";
    for (int i = 1; i <= 165; ++i) {
        cout << i + 165 << " 496\n";
        cout << i + 165 << " " << i << "\n";
        cout << i << " " << i + 165 * 2 << "\n";
        cout << i + 165 * 2 << " 497\n";
    }

    cout << "496 498\n";
}
