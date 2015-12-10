#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <array>

using namespace std;

static const int kInfinite = numeric_limits<int>::max() / 2;

void minimize(int &x, int y) {
    if (y < x)
        x = y;
}

int main() {
    ifstream cin("mesaj.in");
    ofstream cout("mesaj.out");

    int N, M; cin >> N >> M;

    string S; cin >> S;
    vector<string> words(N);
    for (int i = 0 ; i < N; ++i)
        cin >> words[i];

    vector< array<int, 26> > nextChar(M);
    for (int i = 0; i < 26; ++i)
        nextChar[M - 1][i] = M;
    nextChar[M - 1][int(S.back() - 'a')] = M - 1;

    for (int i = M - 2; i >= 0; --i) {
        nextChar[i] = nextChar[i + 1];
        nextChar[i][int(S[i] - 'a')] = i;
    }

    vector<int> best(M + 1, 0);
    for (int i = M - 1; i >= 0; --i) {
        best[i] = 1 + best[i + 1];
        for (int j = 0; j < N; ++j) {
            int last = i, k;
            for (k = 0; k < int(words[j].size()) && last < M; ++k)
                last = nextChar[last][int(words[j][k] - 'a')] + 1;
            if (k == int(words[j].size()) && last <= M)
                minimize(best[i], best[last] + last - i - words[j].size());
        }
}

    cout << best[0] << "\n";
}
