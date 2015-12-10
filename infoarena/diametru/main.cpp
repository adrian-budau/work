#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ofstream cout("diametru.out");

    cout << "499 745\n";
    for (int i = 1; i <= 248; ++i) {
        cout << i << " 497\n";
        cout << i + 248 << " " << i << "\n";
        cout << i + 248 << " 498\n";
    }

    cout << "497 499\n";
}
