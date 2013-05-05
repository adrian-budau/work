#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

pair<int, int> completed(int N) {
    int A = 1, add = 1, level = 0;
    while (A < N) {
        add += 4;
        if (A + add > N)
            return {level, N - A};
        A += add;
        level += 1;
    }
    return {level, 0};
}

int abs(int x) {
    if (x < 0)
        return -x;
    return x;
}

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N, X, Y; cin >> N >> X >> Y;

        int LEVELS, LEFT;
        tie(LEVELS, LEFT) = completed(N);
        LEVELS *= 2;
        double answer = 0;
        int ME = abs(X) + abs(Y);

        if (ME <= LEVELS)
            answer = 1;
        else if (ME > LEVELS + 2)
            answer = 0;
        else if (X == 0 or LEFT == 0)
            answer = 0;
        else {
            int need = Y + 1;

            LEVELS += 2;
            vector< vector<double> > dp(LEVELS + 1, vector<double>(LEVELS + 1, 0));
            dp[0][0] = 1;
            answer = 0;
            for (int i = 0; i <= LEVELS; ++i)
                for (int j = 0; j <= LEVELS; ++j) {
                    if (i + j == LEFT and i >= need)
                        answer += dp[i][j];
 
                    if (i == LEVELS and j == LEVELS)
                        continue;
                    if (i == LEVELS)
                        dp[i][j + 1] += dp[i][j];
                    else if (j == LEVELS)
                        dp[i + 1][j] += dp[i][j];
                    else {
                        dp[i + 1][j] += dp[i][j] / 2;
                        dp[i][j + 1] += dp[i][j] / 2;
                    }

               }
        }

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(10);
        cout << "Case #" << test << ": " << answer << "\n";
    }
}
