#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

template<class Iterator>
bool is_permutation(Iterator begin, Iterator end) {
    vector<int> values(begin, end);
    sort(values.begin(), values.end());
    for (int i = 1; i < int(values.size()); ++i)
        if (values[i - 1] == values[i])
            return false;
    if (values.front() < 0 || values.back() >= int(values.size()))
        return false;
    return true;
}

int main() {
    ifstream cin("harrypotter.in");
    ofstream cout("harrypotter.out");
    int N; assert(cin >> N);
    assert(1 <= N && N <= 1000);

    N *= 4;
    vector<int> V(N);
    for (int i = 0; i < N; ++i) {
        assert(cin >> V[i]);
        assert(1 <= V[i] && V[i] <= N);
        --V[i];
    }

    for (int i = 0; i < 4; ++i)
        assert(is_permutation(V.begin() + i * N / 4, V.begin() + (i + 1) * N / 4));

    for (int i = 0; i < N; ++i)
        V.push_back(V[i]);

    vector< vector<int> > dp(2 * N, vector<int>(2 * N, 0));
    vector< vector<int> > positions(N / 4);
    for (int i = 0; i < 2 * N; ++i)
        positions[V[i]].push_back(i);

    for (int diff = 1; diff < N; ++diff)
        for (int i = 0; i + diff < 2 * N; ++i) {
            int j = i + diff;
            for (int k = i; k < j; ++k)
                dp[i][j] = max(dp[i][j], dp[i][k] + dp[k + 1][j]);
            if (V[i] == V[j])
                dp[i][j] = max(dp[i][j], dp[i + 1][j - 1] + 1);
        }
    int answer = 0;
    for (int i = 0; i < N; ++i)
        answer = max(answer, dp[i][i + N - 1]);
    cout << answer << "\n";
}
