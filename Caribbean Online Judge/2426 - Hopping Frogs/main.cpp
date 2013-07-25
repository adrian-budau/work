#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>

using namespace std;

int which[8][8];
pair<int, int> location[56];

int toInt(string S) {
    int colors = 0;
    int first = -1, second = -1;
    for (int i = 0; i < 8; ++i)
        if (S[i] == '.') {
            if (first == -1)
                first = i;
            else
                second = i;
        } else if (S[i] == 'R')
            colors = colors * 3;
        else if (S[i] == 'G')
            colors = colors * 3 + 1;
        else
            colors = colors * 3 + 2;
    return which[first][second] * 729 + colors;
}

string toString(int x) {
    string S(' ', 8);

    int order = x / 729;
    int first = location[order].first;
    int second = location[order].second;

    S[first] = S[second] = '.';

    int colors = x % 729;
    for (int i = 7; i >= 0; --i)
        if (S[i] != '.') {
            int v = colors % 3;
            colors /= 3;
            if (v == 0)
                S[i] = 'R';
            else if (v == 1)
                S[i] = 'G';
            else
                S[i] = 'B';
        }
    return S;
}

int main() {
    int k = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = i + 1; j < 8; ++j) {
            which[i][j] = k;
            location[k++] = make_pair(i, j);
        }

    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        string start, finish; cin >> start >> finish;

        int start_state = toInt(start);
        int final_state = toInt(finish);
        queue<int> Q;
        Q.push(start_state);

        vector<int> distance(20412, -1);
        distance[start_state] = 0;

        while (!Q.empty()) {
            int x = Q.front();
            Q.pop();

            if (x == final_state)
                break;

            string S = toString(x);

            for (int i = 0; i < 8; ++i)
                if (S[i] != '.') {
                    int minv = i;
                    int maxv = i;
                    if (S[i] != 'G')
                        maxv = i + 2;
                    if (S[i] != 'B')
                        minv = i - 2;

                    for (int j = max(minv, 0); j <= min(maxv, 7); ++j)
                        if (S[j] == '.') {
                            int dist = i - j;
                            if (dist < 0)
                                dist = -dist;
                            if (dist == 2) {
                                int k = (i + j) / 2;
                                if (S[k] == '.')
                                    continue;
                            }

                            string S2 = S;
                            S2[i] = '.';
                            S2[j] = S[i];
                            int y = toInt(S2);
                            if (distance[y] == -1) {
                                Q.push(y);
                                distance[y] = distance[x] + 1;
                            }
                        }
                }
        }

        cout << "Case " << test << ": ";
        if (distance[final_state] == -1)
            cout << "impossible\n";
        else
            cout << distance[final_state] << "\n";
    }
}
