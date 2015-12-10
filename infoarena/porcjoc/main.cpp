#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int64_t gcd(int64_t a, int64_t b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

int main() {
    ifstream cin("porcjoc.in");
    ofstream cout("porcjoc.out");

    int N; cin >> N;
    vector< pair<int, int> > X(N);

    int totalP = 0;
    for (int i = 0; i < N; ++i) {
        cin >> X[i].first >> X[i].second;
        totalP += X[i].second;
    }

    vector<int> V(max_element(X.begin(), X.end())->first + 1, 0);
    for (auto &p : X)
        V[p.first] += p.second;

    int64_t num = 1LL * totalP * totalP;
    int64_t den = num;
    vector<int> weight(V.size(), 0);
    for (int i = 2; i < int(V.size()); ++i) {
        if (weight[i] < 0)
            continue;
        if (weight[i] == 0) {
            for (int64_t j = 1LL * i * i; j < int(V.size()); j += 1LL * i * i)
                weight[j] = -1;
            for (int j = i; j < int(V.size()); j += i)
                if (weight[j] >= 0)
                    weight[j] += 1;
        }
        int have = 0;
        for (int j = i; j < int(V.size()); j += i)
            have += V[j];

        if (weight[i] % 2 == 1)
            num -= 1LL * have * have;
        else
            num += 1LL * have * have;
    }

    int64_t d = gcd(num, den);
    cout << num / d << "/" << den / d << "\n";
}
