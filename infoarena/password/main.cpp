#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    ifstream cin("password.in");
    ofstream cout("password.out");

    string S; cin >> S;

    S = S + S;
    vector<int> pi(S.size(), 0);
    int k = 0;

    int start = 0;
    for (int i = 1; i < int(S.size()); ++i) {
        while (k and S[start + k] != S[i]) {
            if (S[i] < S[start + k])
                start = i - k;
            k = pi[k - 1];
        }

        if (S[i] == S[start + k])
            ++k;
        else
            if (S[i] < S[start + k])
                start = i;

        pi[i - start] = k;
    }

    cout << start << "\n";
}
