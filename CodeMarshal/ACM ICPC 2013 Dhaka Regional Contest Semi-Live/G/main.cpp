#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

const int kBuffer = 32768;

char s[kBuffer];
int p = kBuffer - 1;

void cit(int &x) {
    x = 0;
    while (s[p] < '0' || s[p] > '9')
        if (++p == kBuffer) {
            fread(s, 1, kBuffer, stdin);
            p = 0;
        }
    while (s[p] >= '0' && s[p] <= '9') {
        x = x * 10 + s[p] - '0';
        if (++p == kBuffer) {
            fread(s, 1, kBuffer, stdin);
            p = 0;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    int T; cit(T);

    for (int test = 1; test <= T; ++test) {
        cout << "Case " << test << ":\n";

        int N, M; cit(N); cit(M);

        vector< pair<int, int> > A(N);
        for (int i = 0; i < N; ++i) {
            cit(A[i].first);
            cit(A[i].second);
        }
        sort(A.begin(), A.end(), [](pair<int, int> A, pair<int, int> B) {
            if (A.first != B.first)
                return A.first < B.first;
            return A.second > B.second;
        });

        {
            vector< pair<int, int> > B;
            for (int i = 0; i < N; ++i) {
                if (B.size())
                    if (B.back().first <= A[i].first && B.back().second >= A[i].second)
                        continue;
                B.push_back(A[i]);
            }
            swap(A, B);
        }

        N = A.size();
        while (M--) {
            int x; cit(x);

            int first = lower_bound(A.begin(), A.end(), x, [&](pair<int, int> interval, int punct) {
                return interval.second < punct;
            }) - A.begin();

            if (first == N) {
                cout << "0\n";
                continue;
            }

            if (A[first].first > x) {
                cout << "0\n";
                continue;
            }

            int second = lower_bound(A.begin() + first, A.end(), x, [&](pair<int, int> interval, int punct) {
                return abs(punct - interval.first) > abs(punct - interval.second);
            }) - A.begin();

            int answer = 0;
            for (int i = max(second - 1, first); i < second + 1 && i < N; ++i)
                if (A[i].first <= x && x <= A[i].second)
                    answer = max(answer, min(abs(A[i].first - x), abs(A[i].second - x)));

            cout << answer << "\n";
        }
    }
}
