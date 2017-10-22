#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("puzzle2.in");
    ofstream cout("puzzle2.out");

    int N, M; assert(cin >> N >> M);
    assert(1 <= N && N <= 100 * 1000);
    vector< vector<int> > edges(N);
    for (int i = 0; i < M; ++i) {
        int x, y; assert(cin >> x >> y);
        assert(1 <= x && x <= N && 1 <= y && y <= N);
        assert(x != y);
        edges[x - 1].emplace_back(y - 1);
        edges[y - 1].emplace_back(x - 1);
    }

    auto degree = [&](int node) -> int {
        return edges[node].size();
    };

    if (N == 1) {
        cout << "1 1\n1\n";
        return 0;
    }

    vector< vector<int> > puzzle;
    vector<bool> used(N, false);
    for (int i = 0; i < N; ++i)
        if (degree(i) == 1) {
            puzzle = vector< vector<int> > (N, vector<int>(1));
            puzzle[0][0] = i;
            used[i] = true;
            break;
        }

    if (puzzle.empty()) {
        int corner = 0;
        for (int i = 1; i < N; ++i)
            if (degree(i) == 2) {
                corner = i;
                break;
            }

        // Check if we have special 2 by N case
        int second_corner = -1;
        for (auto &neighbour : edges[corner])
            if (degree(neighbour) == 2)
                second_corner = neighbour;
        if (second_corner != -1) {
            puzzle  = vector< vector<int> >(N / 2, vector<int>(2));
            puzzle[0] = {corner, second_corner};
            used[corner] = used[second_corner] = true;
        } else {
            vector<int> topRow;
            for (int i = corner; i != -1;) {
                int next = -1;
                topRow.push_back(i);
                used[i] = true;
                for (auto &x : edges[i])
                    if (!used[x] && degree(x) == 3) {
                        next = x;
                        break;
                    }
                i = next;
            }
            for (auto &x : edges[topRow.back()])
                if (!used[x] && degree(x) == 2) {
                    topRow.push_back(x);
                    used[x] = true;
                    break;
                }
            puzzle = vector< vector<int> >(N / topRow.size(), vector<int>(topRow.size()));
            puzzle[0] = topRow;
        }
    }
   for (int i = 1; i < int(puzzle.size()); ++i)
        for (int j = 0; j < int(puzzle[i].size()); ++j)
            for (auto &x : edges[puzzle[i - 1][j]])
                if (!used[x]) {
                    used[x] = true;
                    puzzle[i][j] = x;
                }
    cout << puzzle.size() << " " << puzzle[0].size() << "\n";
    for (int i = 0; i < int(puzzle.size()); ++i) {
        for (int j = 0; j < int (puzzle[0].size()); ++j)
            cout << puzzle[i][j] + 1 << " ";
        cout << "\n";
    }
}
