#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
    int T; cin >> T;

    vector<int> good(27);
    good[26] = 0;
    for (int i = 0; i < 26; ++i)
        good[i] = (1 << i);

    for (int test = 1; test <= T; ++test) {
        string S; cin >> S;

        map<int, int> M;
        int mask = 0;
        M[mask]++;

        int answer = 0;
        for (int i = 0; i < int(S.size()); ++i) {
            mask ^= (1 << (S[i] - 'a'));
            for (int j = 0; j < 27; ++j)
                answer += M[mask ^ good[j]];
            M[mask]++;
        }

        cout << "Case " << test << ": " << answer << "\n";
    }
}
