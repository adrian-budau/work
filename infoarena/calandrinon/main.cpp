#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

int main() {
    ifstream cin("calandrinon.in");
    ofstream cout("calandrinon.out");

    int N; cin >> N;
    string S; cin >> S;

    vector<int> count(26, 0);
    for (int i = 0; i < N; ++i)
        ++count[S[i] - 'a'];

    int t = 0;
    for (int i = 0; i < 26; ++i)
        t += (count[i] > 0);

    int start = 0;
    for (int i = 0; i < t; ++i) {
        vector<int> aux(26, 0);
        char best = 'z' + 1;
        int where = -1;
        for (int j = start; j < N; ++j) {
            ++aux[S[j] - 'a'];
            if (count[S[j] - 'a'] > 0 && S[j] < best) {
                best = S[j];
                where = j;
            }
            if (aux[S[j] - 'a'] == count[S[j] - 'a']) {
                ++j;
                break;
            }
        }
        cout << best;

        count[best - 'a'] = -1;
        for (int j = start; j <= where; ++j)
            --count[S[j] - 'a'];
        start = where + 1;
    }
    cout << "\n";
}
