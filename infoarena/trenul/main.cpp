#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("trenul.in");
    ofstream cout("trenul.out");

    int N, M; cin >> N >> M;

    int x = 1, y = N;
    for (int i = 0; i < M; ++i) {
        int v; cin >> v;
        if (v == 2) {
            for (int i = 0; i < v; ++i)
                cout << x++ << " ";
            cout << "\n";
        } else {
            for (int i = 0; i < v; ++i)
                cout << y-- << " ";
            cout << "\n";
        }
    }
}
