#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        cout << "Case #" << test << ":\n";
        int N; cin >> N;
        string W; cin >> W;

        vector<int> score(N, 0);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (W[i] == 'R' && W[j] == 'S')
                    ++score[i];
                else if (W[i] == 'P' && W[j] == 'R')
                    ++score[i];
                else if (W[i] == 'S' && W[j] == 'P')
                    ++score[i];

        int maxt = *max_element(score.begin(), score.end());
        int answer = count(score.begin(), score.end(), maxt);
        cout << answer << "\n";
    }
}
