#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream cin("psychtraining.in");
    ofstream cout("psychtraining.out");

    int N; cin >> N;

    cout << 10 << "\n";
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < N; ++j)
            if ((1 << i) & j)
                cout << 'a';
            else
                cout << 'b';
        cout << "\n";
    }
}
