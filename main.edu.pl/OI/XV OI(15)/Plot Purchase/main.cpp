#include <iostream>
#include <vector>
#include <stack>
#include <cassert>

using namespace std;

struct Rectangle {
    Rectangle(const int &_x1 = 0,const int &_y1 = 0, const int &_x2 = 0, const int &_y2 = 0):
            x1(_x1),
            y1(_y1),
            x2(_x2),
            y2(_y2) {
    }

    long long sum(vector< vector<long long> > &partial_sums) {
        assert(x1 >= 0 and x1 <= x2 and x2 < int(partial_sums.size()));
        assert(y1 >= 0 and y1 <= y2 and y2 < int(partial_sums.size()));
        long long answer = partial_sums[x2][y2];
        if (x1 > 0)
            answer -= partial_sums[x1 - 1][y2];
        if (y1 > 0)
            answer -= partial_sums[x2][y1 - 1];
        if (x1 > 0 and y1 > 0)
            answer += partial_sums[x1 - 1][y1 - 1];
        return answer;
    }

    int x1, y1, x2, y2;
};

int main() {
    ios::sync_with_stdio(false);
    int K, N; cin >> K >> N;

    vector< vector<int> > A(N, vector<int>(N, 0)), B = A;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            cin >> A[i][j];
            if ((A[i][j] + 1) / 2 > K)
                B[i][j] = 1;
        }

    vector<int> height(N + 1, 0);

    vector< vector<long long> > partial_sums(N, vector<long long>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            partial_sums[i][j] = A[i][j];
            if (i > 0)
                partial_sums[i][j] += partial_sums[i - 1][j];
            if (j > 0)
                partial_sums[i][j] += partial_sums[i][j - 1];
            if (i > 0 and j > 0)
                partial_sums[i][j] -= partial_sums[i - 1][j - 1];
        }
    }
    Rectangle answer(-1, -1, -1, -1);

    for (int i = 0; i < N; ++i) {
        int goodBottom = -1;
        for (int j = 0; j < N; ++j)
            if (B[i][j] == 0)
                ++height[j];
            else
                height[j] = 0;

        stack<int> S;
        for (int j = 0; j <= N; ++j) {
            while (S.size() and height[S.top()] > height[j]) {
                int much = height[S.top()];
                assert(much > 0);
                S.pop();
                int last;
                if (S.size())
                    last = S.top();
                else
                    last = -1;

                if (last < goodBottom) {
                    Rectangle aux(i - much + 1, last + 1, i, j - 1);
                    if (aux.sum(partial_sums) >= K)
                        answer = aux;
                }
            }

            while (S.size() and height[S.top()] == height[j])
                S.pop();

            S.push(j);

            if (i < N - 1 and j < N and B[i + 1][j] == 1)
                goodBottom = j;
            if (i == N - 1)
                goodBottom = j;
        }
    }

    if (answer.x1 == -1) {
        cout << "NIE\n";
        return 0;
    }

    while (answer.sum(partial_sums) > 2LL * K) {
        if (answer.x2 - answer.x1 == 0)
            break;
        --answer.x2;
        if (answer.sum(partial_sums) < K) {
            answer = Rectangle(answer.x2 + 1, answer.y1, answer.x2 + 1, answer.y2);
            break;
        }
    }

    while (answer.sum(partial_sums) > 2LL * K)
        --answer.y2;

    cout << answer.y1 + 1 << " " << answer.x1 + 1 << " " << answer.y2 + 1 << " " << answer.x2 + 1 << "\n";
}
