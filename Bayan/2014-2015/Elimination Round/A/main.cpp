#include <iostream>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N; cin >> N;
        string W; cin >> W;

        int R = 0, P = 0, S = 0;
        for (int i = 0; i < N; ++i)
            if (W[i] == 'R')
                ++R;
            else if (W[i] == 'P')
                ++P;
           else
                ++S;

        cout << "Case #" << test << ":\n";

        int scoreR = S, scoreP = R, scoreS = P;
        int maxScore = 0;
        if (S)
            maxScore = max(maxScore, scoreS);
        if (P)
            maxScore = max(maxScore, scoreP);
        if (R)
            maxScore = max(maxScore, scoreR);

        int answer = 0;
        if (scoreR == maxScore)
            answer += R;
        if (scoreP == maxScore)
            answer += P;
        if (scoreS == maxScore)
            answer += S;
        cout << answer << "\n";
    }
}
