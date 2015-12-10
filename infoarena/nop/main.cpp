#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("nop.in");
    ofstream cout("nop.out");

    int T; cin >> T;
    while (T--) {
        int C; cin >> C;

        vector< vector<int> > answer(40, vector<int>(40, 0));
        for (int i = 0; i < 40; ++i)
            answer[i][0] = answer[0][i] = 1;
        answer[2][2] = answer[2][3] = answer[3][2] = answer[3][3] = 1;
        // then we alternate
        // 3 4 -> 5 5 -> 6 7 -> 8 8
        for (int row = 3, col = 4; row + 3 < 40 && col + 2 < 40; row += 3, col += 3) {
            answer[row][col] = answer[row][col + 1] = answer[row + 1][col] = answer[row + 1][col + 1] = 1;
            answer[row + 2][col + 1] = answer[row + 3][col + 1] = answer[row + 2][col + 2] = answer[row + 3][col + 2] = 1;
        }

        // power 25 is on column 2
        // power 24 is on row 3 
        // power 23 is on column 5
        // power 22 is on row 6
        // power 21 is on column 8
        // ....
        // ...
        // power 2 is on row 36
        // power 1 is on column 38
        // power 0 is on row 39
        for (int i = 0; i < 25; ++i)
            if ((1 << i) & C) {
                if (i % 2 == 1) {
                    int col = 38 - (i / 2) * 3;
                    for (int j = 1; j < 40 && answer[j][col] == 0; ++j)
                        answer[j][col] = 1;
                } else {
                    int row = 39 - (i / 2) * 3;
                    for (int j = 1; j < 40 && answer[row][j] == 0; ++j)
                        answer[row][j] = 1;
                }
            }

        if ((1 << 25) & C)
            answer[1][2] = 1;

        cout << 40 << " " << 40 << "\n";
        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 40; ++j)
                cout << answer[i][j];
            cout << "\n";
        }
    }
}
