#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("ab.in");
    ofstream cout("ab.out");

    int T; cin >> T;

    while (T--) {
        int M, N, K; cin >> M >> N >> K;

        vector<int> S(K);
        for (int i = 0; i < K; ++i)
            cin >> S[i];

        sort(S.begin(), S.end());
        vector<int> B(M);
        for (int i = 0; i < M; ++i) {
            cin >> B[i];
            if (i > 0)
                B[i] += B[i - 1];
        }

        int answer = numeric_limits<int>::max();
        for (int i = 0; i < M; ++i) {
            int first = i + 1 + N - B[i];
            int second = M - i - 1;
            int third = B[i];
            if (first < second)
                swap(first, second);
            if (first < third)
                swap(first, third);
            if (second < third)
                swap(second, third);

            if (third > 0 && K == 2)
                continue;
            if (K == 2)
                answer = min(answer, first * S[0] + second * S[1]);
            else
                answer = min(answer, first * S[0] + second * S[1] + third * S[2]);
        }

        cout << answer << "\n";
    }
}
