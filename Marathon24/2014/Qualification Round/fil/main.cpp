#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int T; cin >> T;

    while (T--) {
        int K; cin >> K;
        string S; cin >> S;

        int left = 0, right = 0;
        while (S.back() == 'D') {
            ++right;
            S.pop_back();
        }

        reverse(S.begin(), S.end());
        while (S.back() == 'D') {
            ++left;
            S.pop_back();
        }

        vector<int> parts;
        for (int i = 0; i < int(S.size()); ++i)
            if (S[i] == 'D') {
                int now = 1;
                while (S[i + 1] == 'D') {
                    ++now;
                    ++i;
                }

                parts.push_back(now);
            }
        sort(parts.begin(), parts.end(), greater<int>());

        if (K == 0) {
            int answer = max(left, right);
            if (parts.size())
                answer = max(answer, parts.front());
            cout << answer << "\n";
            continue;
        }

        int answer = 0;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                int total = (i > 0) * left + (j > 0) * right;
                int cutsLeft = K - (i > 1) - (j > 1);
                int extra = (i != 1) + (j != 1);
                if (extra == 1 && cutsLeft <= 2)
                    --extra;
                if (cutsLeft < 0)
                    continue;
                for (int k = 0; k < int(parts.size()) && cutsLeft; ++k)
                    if (extra) {
                        --extra;
                        total += parts[k];
                        cutsLeft--;
                    } else {
                        if (cutsLeft == 1)
                            break;
                        total += parts[k];
                        cutsLeft -= 2;
                    }
                answer = max(answer, total);
            }
        cout << answer << "\n";
    }
}

