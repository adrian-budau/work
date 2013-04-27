#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    int T; cin >> T;
    for (int i = 1; i <= T; ++i) {
        int N, M; cin >> N >> M;

        vector< vector<int> > A(N, vector<int>(M));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                cin >> A[i][j];

        bool ok = true;
        while (ok) {
            int minim = 500;
            int x = -1, y = -1;

            for (int i = 0; i < N; ++i)
                for (int j = 0; j < M; ++j)
                    if (A[i][j] != -1 && A[i][j] < minim) {
                        minim = A[i][j];
                        x = i; y = j;
                    }

            if (minim == 500)
                break;

            bool row = true, column = true;
            for (int i = 0; i < N; ++i)
                if (A[i][y] != -1 && A[i][y] != minim) {
                    column = false;
                    break;
                }
            
            for (int j = 0; j < M; ++j)
                if (A[x][j] != -1 && A[x][j] != minim) {
                    row = false;
                    break;
                }

            if (!row and !column) {
                ok = false;
                break;
            }

            if (row) {
                for (int j = 0; j < M; ++j)
                    A[x][j] = -1;
            } else {
                for (int i = 0; i < N; ++i)
                    A[i][y] = -1;
            }
        }

        cout << "Case #" << i << ": ";
        if (ok)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
}
