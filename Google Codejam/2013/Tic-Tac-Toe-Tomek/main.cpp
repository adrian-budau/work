#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool win(vector<string> &A, char c) {
    for (int i = 0; i < 4; ++i)
        if (A[i] == string(4, c))
            return true;

    for (int j = 0; j < 4; ++j) {
        bool ok = true;
        for (int i = 0; i < 4; ++i)
            if (A[i][j] != c)
                ok = false;
        if (ok)
            return true;
    }

    bool ok = true;
    for (int j = 0; j < 4; ++j)
        if (A[j][j] != c)
            ok = false;
    if (ok)
        return true;
    
    ok = true;
    for (int j = 0; j < 4; ++j)
        if (A[j][4 - j - 1] != c)
            ok = false;
    return ok;
}

int main() {
    int T; cin >> T;
    for (int i = 1; i <= T; ++i) {
        vector<string> A(4);
        for (int j = 0; j < 4; ++j)
            cin >> A[j];

        vector<string> B = A;

        bool ended = true;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                if (A[i][j] == 'T') {
                    A[i][j] = 'X';
                    B[i][j] = 'O';
                }

                if (A[i][j] == '.')
                    ended = false;
            }

        cout << "Case #" << i << ": ";

        if (win(A, 'X')) {
            cout << "X won\n";
            continue;
        }

        if (win(B, 'O')) {
            cout << "O won\n";
            continue;
        }

        if (ended) {
            cout << "Draw\n";
            continue;
        }

        cout << "Game has not completed\n";
    }
}
