#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

string answer(string P, int from, int to, const string& S, const map<string, vector<int> > &M) {
    while (P.size() && P[0] != '*') {
        if (P[0] != S[from])
            return "No";
        P.erase(P.begin());
        if (from > to)
            return "No";
        ++from;
    }
    while (P.size() && P[P.size() - 1] != '*') {
        if (P.back() != S[to])
            return "No";
        P.erase(P.begin() + P.size() - 1);
        if (from > to)
            return "No";
        --to;
    }
    if (count(P.begin(), P.end(), '*') == int(P.size())) {
        if (from <= to && P.size() == 0)
            return "No";
        else
            return "Yes";
    }

    while (!P.empty()) {
        if (P[0] == '*') {
            P.erase(P.begin());
            continue;
        }
        string Q;
        while (P[0] != '*') {
            Q += P[0];
            P.erase(P.begin());
        }
        if (M.count(Q) == 0) {
            return "No";
        }
        const vector<int>& vec = M.find(Q)->second;
        auto pos = lower_bound(vec.begin(), vec.end(), from);
        if (pos == vec.end())
            return "No";
        from = *pos + Q.size();
        if (from > to + 1)
            return "No";
    }
    return "Yes";
}

int main() {
    ios::sync_with_stdio(false);

    string S; cin >> S;
    map<string, vector<int> > M;
    for (int i = 0; i < int(S.size()); ++i) {
        string Q;
        for (int j = i; j < i + 10; ++j) {
            Q += S[j];
            M[Q].push_back(i);
        }
    }

    int N; cin >> N;
    while (N--) {
        int L, R; cin >> L >> R;
        --L; --R;
        string P; cin >> P;

        cout << answer(P, L, R, S, M) << "\n";
    }
}
