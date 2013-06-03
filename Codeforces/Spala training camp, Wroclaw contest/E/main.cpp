#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    int N; cin >> N;

    for (int i = 0; i < N; ++i) {
        string S, T; cin >> S >> T;
        cout << "Case " << i + 1 << ": ";
        if (S.size() != T.size()) {
            cout << "-1\n";
            continue;
        }

        if (count(S.begin(), S.end(), '0') + count(S.begin(), S.end(), '?') < count(T.begin(), T.end(), '0')) {
            cout << "-1\n";
            continue;
        }

        int zero1 = 0, izero = 0, iunu = 0, unuzero = 0;
        for (int i = 0; i < int(S.size()); ++i)
            if (S[i] == '?')
                if (T[i] == '0')
                    ++izero;
                else
                    ++iunu;
            else if (S[i] == '0' and T[i] == '1')
                ++zero1;
            else if (S[i] == '1' and T[i] == '0')
                ++unuzero;
        int need = count(T.begin(), T.end(), '0') - count(S.begin(), S.end(), '0');

        int x = min(izero, need);
        need -= x;
        izero -= x;

        int cost = 0; 
        cost = need + zero1 + count(S.begin(), S.end(), '?');

        cout << cost << "\n";
    }
}
