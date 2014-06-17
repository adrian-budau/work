#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream cin("adunare.in");
    ofstream cout("adunare.out");

    int A, B; cin >> A >> B;

    cout << A + B << "\n";
}
