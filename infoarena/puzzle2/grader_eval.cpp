#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

static const int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, -1, 0, 1};

void result(int score, string message) {
    cout << score;
    cerr << message;
    exit(0);
}

int main() {
    ifstream input("puzzle2.in");
    ifstream output("puzzle2.out");

    if (!output.is_open()) {
        result(0, "Fisier de iesire lipsa");
    }

    int N, M; input >> N >> M;
    vector< vector<int> > edges(N);
    for (int i = 0; i < M; ++i) {
        int x, y; input >> x >> y;
        edges[x - 1].emplace_back(y - 1);
        edges[y - 1].emplace_back(x - 1);
    }

    for (int i = 0; i < N; ++i)
        sort(edges[i].begin(), edges[i].end());

    int rows, cols;
    if (!(output >> rows >> cols))
        result(0, "Fisier de iesire incomplet");

    if (rows < 1 || rows > N || cols < 1 || cols > N || 1LL * rows * cols != 1LL * N)
        result(0, "Incorect");

    vector< vector<int> > puzzle(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j) {
            if (!(output >> puzzle[i][j]))
                result(0, "Fisier de iesire incomplet");
            if (puzzle[i][j] < 1 || puzzle[i][j] > N)
                result(0, "Incorect");
            --puzzle[i][j];
        }

    vector< vector<int> > output_edges(N);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            for (int k = 0; k < 4; ++k)
                if (i + dx[k] >= 0 && i + dx[k] < rows && j + dy[k] >= 0 && j + dy[k] < cols)
                    output_edges[puzzle[i][j]].push_back(puzzle[i + dx[k]][j + dy[k]]);

    for (int i = 0; i < N; ++i)
        sort(output_edges[i].begin(), output_edges[i].end());
    if (edges != output_edges)
        result(0, "Incorect");
    result(10, "OK");
}
