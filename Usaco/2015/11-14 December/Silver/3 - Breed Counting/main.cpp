#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("bcount.in");
    ofstream cout("bcount.out");

    int N, Q; cin >> N >> Q;

    vector< vector<int> > count(3, vector<int>(N, 0));
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        count[x - 1][i] = 1;
    }

    for (int i = 0; i < 3; ++i)
        for (int j = 1; j < N; ++j)
            count[i][j] += count[i][j - 1];

    while (Q--) {
        int A, B; cin >> A >> B;
        for (int i = 0; i < 3; ++i) {
            cout << count[i][B - 1] - (A > 1 ? count[i][A - 2] : 0);
            if (i < 2)
                cout << ' ';
        }
        cout << "\n";
    }
}
