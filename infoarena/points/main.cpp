#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

int area(int x1, int y1, int x2, int y2, int x3, int y3) {
    return x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
}

int main() {
    ifstream cin("points.in");
    ofstream cout("points.out");

    int N; cin >> N;

    int count[2][2];
    memset(count, 0, sizeof(count));
    long long answer = 0;
    for (int i = 0; i < N; ++i) {
        int x, y; cin >> x >> y;
        x %= 2; y %= 2;

        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
                if (count[j][k] > 0) {
                    int aux = count[j][k];
                    --count[j][k];
                    for (int p = 0; p < 2; ++p)
                        for (int l = 0; l < 2; ++l)
                            if (area(j, k, p, l, x, y) % 2 == 0)
                                answer += aux * count[p][l];
                    ++count[j][k];
                }
        ++count[x][y];
    }

    cout << answer / 2 << "\n";
}
