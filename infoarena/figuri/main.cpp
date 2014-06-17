#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1}, dy[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

int dfs(vector<vector<int>>& colour, int X, int Y, int col) {
    colour[X][Y] = 0;
    int answer = 1;
    for (int i = 0; i < 8; ++i)
        if (colour[X + dx[i]][Y + dy[i]] == col)
            answer += dfs(colour, X + dx[i], Y + dy[i], col);
    return answer;
}

int main() {
    ifstream cin("figuri.in");
    ofstream cout("figuri.out");

    int A, B, N; cin >> A >> B >> N;

    vector< vector<int> > colour(A + 2, vector<int>(B + 2, 0));
    vector< vector<int> > many(A + 2, vector<int>(B + 2, 0));

    for (int i = 1; i <= A; ++i)
        for (int j = 1; j <= B; ++j)
            colour[i][j] = many[i][j] = 1;

    for (int i = 0; i < N; ++i) {
        int x1, y1, x2, y2, c;
        cin >> x1 >> y1 >> x2 >> y2 >> c;
        x1 += A / 2; ++x1;
        x2 += A / 2; 
        y1 += B / 2; ++y1;
        y2 += B / 2; 

        for (int j = x1; j <= x2; ++j)
            for (int k = y1; k <= y2; ++k) {
                colour[j][k] = c;
                ++many[j][k];
            }
    }

    vector< pair<int, int> > answer;
    int maxt = 0;
    for (int i = 1; i <= A; ++i)
        for (int j = 1; j <= B; ++j) {
            if (colour[i][j]) {
                int col = colour[i][j];
                answer.emplace_back(col, dfs(colour, i, j, col));
            }

            maxt = max(maxt, many[i][j]);
        }

    sort(answer.begin(), answer.end());

    for (auto &p : answer)
        cout << p.first << " " << p.second << "\n";
    cout << maxt << "\n";
}
