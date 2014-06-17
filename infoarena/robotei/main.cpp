#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

int main() {
    ifstream cin("robotei.in");
    ofstream cout("robotei.out");

    int N, M, X, Y, modX, modY, offsetX, offsetY;

    cin >> N >> M >> X >> Y >> modX >> modY >> offsetX >> offsetY;

    if (X >= modX or Y >= modY) {
        if (X < N and Y < N) {
            cout << "0 " << N * N - 1 << "\n";
            cout << "1 1\n";
        } else
            cout << "0 " << N * N << "\n";

        return 0;
    }

    auto next = [&](int X, int Y) -> pair<int, int> {
        int newX = (X * X + offsetX) % modX;
        int newY = (Y * Y + offsetY) % modY;
        return {newX, newY};
    };


    vector< vector< pair<int, int> > > edge(modX, vector< pair<int, int> >(modY));
    for (int i = 0; i < modX; ++i)
        for (int j = 0; j < modY; ++j)
            edge[i][j] = next(i, j);

    vector< vector<int> > distance(modX, vector<int>(modY, -1));

    distance[X][Y] = 0;
    for (int i = 0; i < modX; ++i)
        for (int j = 0; j < modY; ++j) 
            if (distance[i][j] == -1) {
                int curX = i, curY = j;
                int dist = 0;
                for (; (curX != X or curY != Y) and distance[curX][curY] == -1; ++dist) {
                    distance[curX][curY] = -2;
                    tie(curX, curY) = next(curX, curY);
                }

                if (distance[curX][curY] < -1)
                    dist = -3;
                else
                    dist += distance[curX][curY];

                curX = i, curY = j;
                for (; (curX != X or curY != Y) and distance[curX][curY] == -2; --dist) {
                    distance[curX][curY] = dist;
                    tie(curX, curY) = next(curX, curY);
                }
            }

    int nextX, nextY;
    tie(nextX, nextY) = next(X, Y);
    distance[X][Y] = 1 + distance[nextX][nextY];

    vector<int> answer(M + 1, 0);
    for (int i = 0; i < modX; ++i)
        for (int j = 0; j < modY; ++j) {
            int many;
            if (distance[i][j] > 0) {
                many = 1;
                int auxM = M - distance[i][j];
                if (distance[X][Y] > 0)
                    many += auxM / distance[X][Y];
            } else
                many = 0;

            int manyX = N / modX;
            if (i < N % modX)
                ++manyX;
            int manyY = N / modY;
            if (j < N % modY)
                ++manyY;
            answer[many] += manyX * manyY;
        } 

    int many = 0;
    if (distance[X][Y] > 0)
        many = M / distance[X][Y];
    else
        many = 0;
    --answer[many];
    ++answer[many + 1];

    for (int i = 0; i <= M; ++i)
        if (answer[i])
            cout << i << " " << answer[i] << "\n";
}
