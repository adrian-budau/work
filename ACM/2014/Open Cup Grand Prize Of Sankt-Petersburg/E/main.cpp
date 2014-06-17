#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("decimal.in");
    ofstream cout("decimal.out");

    string S; cin >> S;

    if (S[0] == '1') {
        cout << "1.(0)\n";
        return 0;
    }

    auto orig = S;
    reverse(S.begin(), S.end());
    vector<int> match(S.size(), 0);

    int left = 0, right = 0;
    for (int i = 1; i < int(S.size()); ++i)
        if (right <= i) {
            left = i;
            right = i;
            while (right < int(S.size()) && S[right] == S[right - left])
                ++right;
            match[i] = right - left;
        } else {
            if (match[i - left] + i >= right) {
                left = i;
                while (right < int(S.size()) && S[right] == S[right - left])
                    ++right;
                match[i] = right - left;
            } else {
                match[i] = match[i - left];
            }
        }

    int best = -1, when = -1;
    for (int i = 1; S[i - 1] != '.'; ++i)
        if (match[i] > best) {
            best = match[i];
            when = i;
        }

    S = orig;
    cout << S[0] << S[1];
    S = S.substr(2);
    cout << S.substr(0, S.size() - best - when);
    S = S.substr(S.size() - best - when);
    cout << "(" << S.substr(0, when) << ")" << "\n";
}
