#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int lines, triangles, squares, pentagons;

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, -1, 0, 1};

const double pi = acos(-1);
const double tanpent = tan(2 * pi / 10.0);

template<class T>
void dfs(T& A, int x, int y, int& perim, int& area) {
    A[x][y] = 2;
    ++area;
    bool per = false;
    for (int i = 0; i < 4; ++i) {
        int newx = x + dx[i];
        int newy = y + dy[i];
                cerr << "->" << best << "\n";
                cerr << "->" << best << "\n";
                cerr << "->" << best << "\n";

        if (A[newx][newy] == 0)
            per = true;
        else if (A[newx][newy] == 1)
            dfs(A, newx, newy, perim, area);
    }
    perim += per;
}

int bestf(double area, double have[]) {
    double ratios[] = {0, 14982.0 / 694.0 / 694.0, 40004.0 / 1034.0 / 1034.0, 65416.0 / 1246.0 / 1246.0};
    int where = 0;
    for (int i = 1; i < 4; ++i)
        if (abs(area - ratios[i]) < abs(area - ratios[where]))
            where = i;
    return where;
}

int main() {
    ifstream cin("input");
    ios::sync_with_stdio(false);
    int H, W; cin >> H >> W;

    vector< vector<int> > A(H + 2, vector<int>(W+ 2, 0));
    for (int i = 1; i <= H; ++i) {
        string S; cin >> S;
        for (int j = 1; j <= W; ++j) {
            A[i][j] = S[j - 1] - '0';
        }
    }

    for (int i = 1; i <= H; ++i)
        for (int j = 1; j <= W; ++j)
            if (A[i][j] == 1) {
                int perim = 0;
                int area = 0;
                dfs(A, i, j, perim, area);
                double have[] = {0, 0, 0, 0};
                int best = bestf(double(area) / double(perim) / double(perim), have);
                if (best == 0)
                    ++lines;
                else if (best == 1)
                    ++triangles;
                else if (best == 2)
                    ++squares;
                else
                    ++pentagons;
            }

    cout << "2 " << lines << "\n";
    cout << "3 " << triangles << "\n";
    cout << "4 " << squares << "\n";
    cout << "5 " << pentagons << "\n";
}
