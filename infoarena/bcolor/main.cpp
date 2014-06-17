#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("bcolor.in");
    ofstream cout("bcolor.out");

    int N, M, K; cin >> N >> M >> K;
    --K;

    vector< vector<int> > index(N, vector<int>(N, -1)), edge(N, vector<int>(N, 0));
    vector<int> colour(M, 0);
    for (int i = 0; i < M; ++i) {
        int x, y, c; cin >> x >> y >> c;
        --x; --y;
        index[x][y] = index[y][x] = i;
        edge[x][y] = edge[y][x] = 1;
        colour[i] = c;
    }

    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (edge[i][k] and edge[k][j])
                    edge[i][j] = 1;

    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (index[i][j] != -1)
                if (!edge[0][i] && !edge[0][j])
                    colour[index[i][j]] = 1; // we pass 0 times through it

    // now build the gaussian system
    vector<int> answer(M, 0);
    vector< vector<bool> > system(N, vector<bool>(M + 1, 0));
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (index[i][j] != -1) {
                if (colour[index[i][j]] == 2) {
                    system[i][M] = (1 - system[i][M]);
                    system[j][M] = (1 - system[j][M]);
                    answer[index[i][j]] = 2;
                } else if (colour[index[i][j]] == 1)
                    answer[index[i][j]] = 1;
                else {
                    system[i][index[i][j]] = 1;
                    system[j][index[i][j]] = 1;
                }
            }

    int row = 0, col = M - 1;
    vector<bool> freeVariable(M, false);
    while (col >= 0) {
        int next;
        for (next = row; next < N and not system[next][col]; ++next);
        if (next == N) { // yey free variable to play with
            freeVariable[col] = true;
            --col;
            continue;
        }

        swap(system[row], system[next]);
        // fixed variable
        for (int i = 0; i < N; ++i)
            if (i != row and system[i][col])
                for (int j = 0; j <= M; ++j)
                    if (system[row][j])
                        system[i][j] = 1 - system[i][j];
        ++row; --col;
    }

    // first free variables
    for (int i = M - 1; i >= 0; --i)
        if (freeVariable[i])
            if (colour[i] == 0) {
                if (K % 2) // it's red
                    for (int j = 0; j < N; ++j)
                        if (system[j][i])
                            system[j][M] = 1 - system[j][M]; 
                for (int j = 0; j < N; ++j)
                    system[j][i] = 0;
                answer[i] = K % 2 + 1;
                K /= 2;
           }

    // we need to set all those fixed variables
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (system[i][j]) {
                answer[j] = system[i][M] + 1;
                break;
            }

    for (int i = 0; i < M; ++i)
        if (answer[i] == 1)
            cout << "A";
        else
            cout << "R";
}
