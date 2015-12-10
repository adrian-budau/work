#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    ifstream cin("subbit.in");
    ofstream cout("subbit.out");

    string S; cin >> S;

    int position = 0;
    int digit = 0;
    for (int i = 0; position < int(S.size()); ++i) {
        int j;
        for (j = 0; (1 << j) <= i; ++j);
        for (j = max(j - 1, 0); position < int(S.size()) && j >= 0; --j) {
            int bit = bool((1 << j) & i);
            if (S[position] == bit + '0')
                ++position;
            ++digit;
        }
    }

    cout << digit << "\n";
}
