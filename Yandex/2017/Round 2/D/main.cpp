#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

static const int dx[4] = {-1, 0, 1, 0};
static const int dy[4] = {0, -1, 0, 1};

void touch(vector< vector<int> >& mat, int x, int y, int&nonzero) {
    ++mat[x][y];
    if (mat[x][y] == 1) {
        ++nonzero;
        return;
    }

    if (mat[x][y] == 5) {
        mat[x][y] = 0;
        --nonzero;
        for (int i = 0; i < 4; ++i)
            touch(mat, x + dx[i], y + dy[i], nonzero);
    }
}

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    vector< vector<int> > mat(301, vector<int>(301, 0));
    int nonzero = 0;
    for (int i = 0; i < N; ++i) {
        int x, y; cin >> x >> y;

        touch(mat, x + 150, y + 150, nonzero);
        cout << nonzero << "\n";
    }

}

