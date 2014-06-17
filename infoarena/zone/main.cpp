#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <numeric>

using namespace std;

int main() {
    ifstream cin("zone.in");
    ofstream cout("zone.out");

    int N; cin >> N;

    array<int64_t, 9> sum;
    for (int i = 0; i < 9; ++i)
        cin >> sum[i];

    vector< vector<int> > V(N, vector<int>(N));
    vector< vector<int64_t> > S(N, vector<int64_t>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            cin >> V[i][j];
            S[i][j] = V[i][j];
            if (i > 0)
                S[i][j] += S[i - 1][j];
            if (j > 0)
                S[i][j] += S[i][j - 1];
            if (i > 0 and j > 0)
                S[i][j] -= S[i - 1][j - 1];
        }

    auto partial = [&](int x1, int y1, int x2, int y2) {
        int64_t answer = S[x2][y2];
        if (x1 > 0)
            answer -= S[x1 - 1][y2];
        if (y1 > 0)
            answer -= S[x2][y1 - 1];
        if (x1 > 0 and y1 > 0)
            answer += S[x1 - 1][y1 - 1];
        return answer;
    };

    vector<int> answer{N, N, N, N};

    sort(sum.begin(), sum.end());

    auto check_answer = [&](int L1, int L2, int C1, int C2) {
        int L[4] = {0, L1, L2, N};
        int C[4] = {0, C1, C2, N};

        array<int64_t, 9> now_sum;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                now_sum[i * 3 + j] = partial(L[i], C[j], L[i + 1] - 1, C[j + 1] - 1);
        sort(now_sum.begin(), now_sum.end());
        if (now_sum != sum)
            return;
        answer = min<vector<int>>(answer, {L1, C1, L2, C2});
    };

    for (int i = 0; i + 2 < N; ++i) {
        int where = 0;

        if (answer.front() != N)
            break;
        for (int j = 0; j + 2 < N; ++j) {
            int64_t now = partial(0, 0, i, j);
            while (where < 9 and sum[where] < now)
                ++where;
            if (where == 9)
                break;
            if (sum[where] != now)
                continue;

            array<int64_t, 8> left;
            for (int p = 0; p < 9; ++p)
                if (p < where)
                    left[p] = sum[p];
                else if (p > where)
                    left[p - 1] = sum[p];

            int whereleft = 0;
            for (int i2 = i + 1; i2 + 1 < N; ++i2) {
                int64_t nowleft = partial(i + 1, 0, i2, j);
                while (whereleft < 8 and left[whereleft] < nowleft)
                    ++whereleft;
                if (whereleft == 8)
                    break;
                if (left[whereleft] != nowleft)
                    continue;

                array<int64_t, 7> up;
                for (int p = 0; p < 8; ++p)
                    if (p < whereleft)
                        up[p] = left[p];
                    else if (p > whereleft)
                        up[p - 1] = left[p];

                int whereup = 0;
                for (int j2 = j + 1; j2 + 1 < N; ++j2) {
                    int64_t nowup = partial(0, j + 1, i, j2);
                    while (whereup < 7 and up[whereup] < nowup)
                        ++whereup;
                    if (whereup == 7)
                        break;
                    if (up[whereup] != nowup)
                        continue;

                    check_answer(i + 1, i2 + 1, j + 1, j2 + 1);
                }
            }
        }
    }
    cout << answer[0] << " " << answer[2] << " " << answer[1] << " " << answer[3] << "\n";
}
