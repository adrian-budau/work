#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ios::sync_with_stdio(false);

    string S; cin >> S;

    int64_t answer = 0;
    vector<int> pi(S.size(), 0), seen_middle(S.size() + 1, 0);
    int K = 0;
    for (int i = 1; i < int(S.size()); ++i) {
        while (K > 0 && S[i] != S[K])
            K = pi[K - 1];
        if (S[i] == S[K])
            ++K;
        pi[i] = K;

        for (int j = K; j > 0; j = pi[j - 1]) {
            if (seen_middle[j]) {
                answer += j;
                break;
            }
            seen_middle[j] = 1;
        }
    }

    cout << answer << "\n";
}
