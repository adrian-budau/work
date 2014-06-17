#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

int main() {
    int N; cin >> N;
    string word; cin >> word;

    vector<string> S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i];

    long long answer = 0;
    int M = N - 1;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            if (i != 0 && j != 0 && i != M && j != M)
                continue;

            for (int k = 0; k < 8; ++k) {
                string current = "";
                int curi = i;
                int curj = j;
                if (i - dx[k] >= 0 && i - dx[k] < N && j - dy[k] >= 0 && j - dy[k] < N)
                    continue;
                while (curi >= 0 && curi < N && curj >= 0 && curj < N) {
                    current += S[curi][curj];
                    curi += dx[k];
                    curj += dy[k];
                }

                vector<long long> dp(word.size() + 1, 0);
                dp[0] = 1;
                for (string::iterator it = current.begin(); it != current.end(); ++it)
                    for (int i = word.size() - 1; i >= 0; --i)
                        if (word[i] == *it)
                            dp[i + 1] += dp[i];
                answer += dp[word.size()];
            }
        }

    cout << answer << "\n";
}
