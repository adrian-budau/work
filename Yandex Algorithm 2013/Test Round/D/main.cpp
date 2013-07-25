#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("proddiff.in");
    ofstream cout("proddiff.out");

    int64_t N; cin >> N;

    vector<int64_t> factors;
    for (int i = 1; i * i <= N; ++i)
        if (N % i == 0) {
            factors.push_back(i);
            if (i * i != N)
                factors.push_back(N / i);
        }

    sort(factors.begin(), factors.end());

    int K = factors.size();
    vector< vector<int> > most(K, vector<int>(K, 0));

    map<int64_t, int> which;
    for (int i = 0; i < K; ++i)
        which[factors[i]] = i;

    most[0][0] = 1;
    for (int i = 0; i < K; ++i)
        for (int j = 0; j + 1 < K; ++j) {
            if (most[i][j] == 0)
                continue;
            most[i][j + 1] = max(most[i][j + 1], most[i][j]);
            int64_t next = 1LL * factors[i] * factors[j + 1];
            if (which.count(next))
                most[which[next]][j + 1] = max(most[which[next]][j + 1], most[i][j] + 1);
        }

    vector<int64_t> answer;
    int factor = K - 1, much = K - 1;
    while (factor > 0 and much > 0)
        if (most[factor][much] == most[factor][much - 1])
            --much;
        else {
            answer.push_back(factors[much]);
            int64_t now = factors[factor] / factors[much];
            --much;
            factor = which[now];
        }

    answer.push_back(1);
    reverse(answer.begin(), answer.end());

    cout << answer.size() << "\n";
    for (auto &x : answer)
        cout << x << " ";
    cout << "\n";
}
