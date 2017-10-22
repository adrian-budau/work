#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

pair< vector<int>, vector<int> > normalize(vector<int> A, vector<int> B) {
    int pos = min_element(B.begin(), B.end()) - B.begin();
    rotate(A.begin(), A.begin() + pos, A.end());
    rotate(B.begin(), B.begin() + pos, B.end());
    vector<int> newA, newB;
    newA.push_back(A[0]);
    newB.push_back(B[0]);
    int N = A.size();
    for (int i = 1; i < N; ++i)
        if (newA.back() + newB.back() <= B[i]) {
            newA.back() += A[i];
        } else {
            newA.push_back(A[i]);
            newB.push_back(B[i]);
        }

    return make_pair(newA, newB);
}

int64_t best(vector<int> A, vector<int> B, int K) {
    int N = A.size();
    int64_t sum = 0;
    for (auto &x : A)
        sum += x;
    sum += *max_element(B.begin(), B.end());
    int64_t step;
    for (step = 1; step < sum; step <<= 1);
    int64_t answer;

    for (int i = 0; i < N; ++i) {
        A.push_back(A[i]);
        B.push_back(B[i]);
    }

    vector<int> dp(N, -1), many(N, -1);
    for (answer = 0; step; step >>= 1) {
        answer += step;
        fill(dp.begin(), dp.end(), -1);

        int right = N; int64_t dist = 0;
        while (right < 2 * N && dist + A[right] <= answer) {
            dist += A[right];
            ++right;
        }
        for (int i = N - 1; i >= 0; --i) {
            dist += A[i];
            while (right > i && dist + B[i] > answer) {
                --right;
                dist -= A[right];
            }

            if (dist + B[i] <= answer) {
                if (right >= N) {
                    dp[i] = right - N;
                    many[i] = 1;
                } else if (dp[right] != -1) {
                    dp[i] = dp[right];
                    many[i] = many[right] + 1;
                }
            }
        }

        bool found = false;
        for (int i = 0; i < N; ++i)
            if (dp[i] >= i && many[i] <= K)
                found = true;
        if (found)
            answer -= step;
    }
    return answer + 1;
}

int main() {
    ifstream cin("oneouts.in");
    ofstream cout("oneouts.out");

    int N, K; assert(cin >> N >> K);
    assert(1 <= N && N <= 100 * 1000);

    vector<int> A(N), B(N);
    for (int i = 0; i < N; ++i) {
        assert(cin >> A[i]);
        assert(1 <= A[i] && A[i] <= 1000 * 1000 * 1000);
    }

    for (int j = 0; j < N; ++j) {
        assert(cin >> B[j]);
        assert(1 <= B[j] && B[j] <= 1000 * 1000 * 1000);
    }

    tie(A, B) = normalize(A, B);
    cerr << A.size() << "\n";
    cout << best(A, B, min(K, int(A.size()))) << "\n";
}
