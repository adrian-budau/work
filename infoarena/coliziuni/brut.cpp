#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cmath>
#include <tuple>
#include <limits>

using namespace std;

int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, -1, 0, 1};
map<char, int> dir = {{'^', 0}, {'<', 1}, {'v', 2}, {'>', 3}};

int check(int dir1, int x1, int y1, int dir2, int x2, int y2, int N) {
    if (x1 == x2 && y1 == y2)
        return 0;

    if (dir1 % 2 && dir2 % 2) {
        if (x1 != x2)
            return numeric_limits<int>::min();
        if (dir1 == 1 && dir2 == 1) {
            if (y1 < y2)
                return y1 + check(3, x1, 1, 1, x2, y2 - y1, N);
            else
                return y2 + check(1, x1, y1 - y2, 3, x2, 1, N);
        } else if (dir1 == 3 && dir2 == 3) {
            if (y1 < y2)
                return (N + 1 - y2) + check(3, x1, y1 + (N + 1 - y2), 1, x2, N, N);
            else
                return (N + 1 - y1) + check(1, x1, N, 3, x2, y2 + (N + 1 - y1), N);
        } else if (dir1 == 1 && dir2 == 3) {
            if (y1 < y2) {
                :vsp
        }
    }
}

int main() {
    ifstream cin("coliziuni.in");
    ofstream cout("coliziuni.out");

    int N, M; cin >> N >> M;
    vector<tuple<int, int, int>> ants;
    for (int i = 0; i < M; ++i) {
        char direction;
        int x, y;
        cin >> direction >> x >> y;
        ants.emplace_back(direction, x, y);
    }

    int answer = -1;
    for (auto &a1 : ants)
        for (auto &a2 : ants) {
            int dir1, x1, y1, dir2, x2, y2;
            tie(dir1, x1, y1) = a1;
            tie(dir2, x2, y2) = a2;

            int aux = check(dir1, x1, y1, dir2, x2, y2, N);
            answer = max(answer, aux);
        }

    cout << answer << "\n";
}
