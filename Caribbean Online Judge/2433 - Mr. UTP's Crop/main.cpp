#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    while (true) {
        int N, M; cin >> N >> M;

        if (N == 0 and M == 0)
            break;

        string F; cin >> F;
        int formula = 0;
        for (string::iterator it = F.begin(); it != F.end(); ++it)
            formula += (1 << (*it - 'a'));

        vector< vector<char> > A(N, vector<char>(M, 0));
        for (int i = 0; i < N; ++i) {
            string S; cin >> S;
            for (int j = 0; j < M; ++j)
                A[i][j] = S[j] - 'a';
        }

        int answer = 0;
        for (int i = 0; i < N; ++i) {
            vector<int> type(M, 0);
            for (int j = i; j < N; ++j) {
                for (int k = 0; k < M; ++k)
                    if ((1 << A[j][k]) & formula)
                        type[k] |= (1 << A[j][k]);
                    else
                        type[k] = -1;

                int last = -1;
                int mask = 0;
                for (int k = 0; k < M; ++k) {
                    if (type[k] == -1) {
                        last = k;
                        mask = 0;
                    } else
                        mask |= type[k];
                    if (mask == formula)
                        answer = max(answer, (j - i + 1) * (k - last));
                }
            }
        }

        cout << answer << "\n";
    }
}
