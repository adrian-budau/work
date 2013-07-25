#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

bool solve(vector<long long> V) {
    if (V.size() <= 2)
        return true;

    int N = V.size();
    while (N > 1)
        if (N % 2)
            return false;
        else
            N /= 2;
    N = V.size();

    map<long long, int> M;
    for (auto &x : V)
        ++M[x];
    for (auto &x : V)
        if (M[x] >= N / 2) {
            M[x] -= N / 2;
            break;
        }

    vector<long long> V2;
    for (auto &p : M)
        for (int j = 0; j < p.second; ++j)
            V2.push_back(p.first);
    if (V2.size() == V.size())
        return false;
    return solve(V2);
}

int main() {
    int N; cin >> N;

    vector<long long> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    sort(V.begin(), V.end());

    if (solve(V))
        cout << "YES\n";
    else
        cout << "NO\n";
}
