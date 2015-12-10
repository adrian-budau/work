#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

using namespace std;

int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, -1, 0, 1};
map<char, int> dir = {{'^', 0}, {'<', 1}, {'v', 2}, {'>', 3}};

int sameLine(vector< pair<int, int> > &vec, int N) {
    sort(vec.begin(), vec.end());
    int answer = 2 * N + 2;
    int rightMost[2] = {-4 * N - 4, -4 * N - 3};
    for (auto &x : vec)
        if (x.second == 0) { // left
            answer = min(answer, (x.first - rightMost[x.first % 2]) / 2);
            rightMost[1 - x.first % 2] = max(rightMost[1 - x.first % 2], -x.first - 1);
        } else {
            answer = min(answer, (2 * N - 1 - x.first - rightMost[1 - x.first % 2]) / 2);
            rightMost[x.first % 2] = max(rightMost[x.first % 2], x.first);
        }
    return answer;
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
        ants.emplace_back(dir[direction], x - 1, y - 1);
    }

    int answer = numeric_limits<int>::max();
    map<int, vector< pair<int, int> > > horizontal, vertical;

    vector<pair<int, int> > antsDir[4];
    for (int i = 0; i < M; ++i) {
        int direction, x, y;
        tie(direction, x, y) = ants[i];
        if (direction % 2 == 0) {
            vertical[y].emplace_back(x, direction / 2);
            antsDir[direction].emplace_back(x, y);
            antsDir[2 - direction].emplace_back(2 * N - x - 1, y);
        } else {
            horizontal[x].emplace_back(y, direction / 2);
            antsDir[direction].emplace_back(x, y);
            antsDir[4 - direction].emplace_back(x, 2 * N - y - 1);
        }
    }

    map<int, vector< pair<int, int> > > diagonals[2][2];    // first dimension is sum, diff dimension
                                                            // second is down or right ants
    auto sum = [&](pair<int, int> ant) {
        return (ant.first + ant.second) % (2 * N);
    };

    auto diff = [&](pair<int, int> ant) {
        return (ant.first - ant.second + 2 * N) % (2 * N);
    };

    // add the vertical ants
    for (int dir = 0; dir < 4; dir += 2) {
        for (auto &ant : antsDir[dir]) {
            diagonals[0][dir / 2][sum(ant)].emplace_back(ant);
            diagonals[1][dir / 2][diff(ant)].emplace_back(ant);
        }
    }

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j)
            for (auto &p : diagonals[i][j])
                sort(p.second.begin(), p.second.end());

    auto norm = [&](int x) {
        if (x < 0)
            x += 2 * N;
        return x;
    };

    for (auto &vec : vertical)
        answer = min(answer, sameLine(vec.second, N));
    for (auto &vec : horizontal)
        answer = min(answer, sameLine(vec.second, N));

    for (auto &ant : antsDir[3]) { // ants going right
        // ants going up
        if (diagonals[1][0].count(diff(ant))) {
            auto& vec = diagonals[1][0][diff(ant)];
            auto it = lower_bound(vec.begin(), vec.end(), ant);
            if (it == vec.end())
                it = vec.begin();
            answer = min(answer, norm(it->first - ant.first));
        }

        // ants going down
        if (diagonals[0][1].count(sum(ant))) {
            auto& vec = diagonals[0][1][sum(ant)];
            auto it = upper_bound(vec.begin(), vec.end(), ant);
            if (it == vec.begin())
                it = vec.end();
            --it;
            answer = min(answer, norm(ant.first - it->first));
        }
    }

    for (auto &ant : antsDir[1]) { // ants going left
        // ants going up
        if (diagonals[0][0].count(sum(ant))) {
            auto& vec = diagonals[0][0][sum(ant)];
            auto it = lower_bound(vec.begin(), vec.end(), ant);
            if (it == vec.end())
                it = vec.begin();
            answer = min(answer, norm(it->first - ant.first));
        }

        // ants going down
        if (diagonals[1][1].count(diff(ant))) {
            auto& vec = diagonals[1][1][diff(ant)];
            auto it = upper_bound(vec.begin(), vec.end(), ant);
            if (it == vec.begin())
                it = vec.end();
            --it;
            answer = min(answer, norm(ant.first - it->first));
        }
    }


    if (answer > 2 * N)
        answer = -1;
    cout << answer << "\n";
}
