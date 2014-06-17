#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    ifstream cin("gold.in");
    ofstream cout("gold.out");

    int N, limit; cin >> N >> limit;

    vector<int> cost(N * 2);
    for (int i = 0; i < N; ++i) {
        cin >> cost[i];
        cost[N + i] = cost[i];
    }

    vector<int> X(N), Y(N);
    for (int i = 0; i < N; ++i)
        cin >> X[i] >> Y[i];

    int answer = 0;
    for (int i = 0; i < N; ++i) {
        vector<int> P;
        vector<double> angle(2 * N);
        for (int j = 0; j < N; ++j)
            if (i != j) {
                angle[j] = atan2(Y[j] - Y[i], X[j] - X[i]);
                angle[N + j] = angle[j] + 2 * M_PI;
                P.push_back(j);
            }

        sort(P.begin(), P.end(), [&](int x, int y) {
            return angle[x] < angle[y];
        });

        P.resize(2 * N - 2);
        for (int i = 0; i < N - 1; ++i) {
            P[N - 1 + i] = P[i] + N;
        }

        int second_pointer = 0;
        int left = 0, right = 0;
        for (int i = 0; i < N - 1; ++i)
            right += cost[P[i]];

        for (int i = 0; i < N - 1; ++i) {
            while (angle[P[second_pointer]] < angle[P[i]] + M_PI) {
                right -= cost[P[second_pointer]];
                left += cost[P[second_pointer++]];
            }

            left -= cost[P[i]];
            if (abs(right - left) <= limit)
                ++answer;
            right += cost[P[i + N - 1]];
        }
    }

    cout << answer / 2 << "\n";
}
