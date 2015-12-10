#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

int main() {
    ifstream cin("drept.in");
    ofstream cout("drept.out");

    int N, K; cin >> N >> K;
    vector< pair<int, int> > P(N);
    for (int i = 0; i < N; ++i)
        cin >> P[i].first >> P[i].second;

    sort(P.begin(), P.end());
    int answer = numeric_limits<int>::max();

    for (int i = 0; i + K <= N; ++i) {
        vector<int> Y;
        for (int j = i; j < N; ++j) {
            Y.insert(lower_bound(Y.begin(), Y.end(), P[j].second), P[j].second);
            if (j - i + 1 < K)
                continue;
            for (int k = 0; k + K <= int(Y.size()); ++k)
                answer = min(answer, (P[j].first - P[i].first) * (Y[k + K - 1] - Y[k]));
        }
    }

    cout << answer << "\n";
}
