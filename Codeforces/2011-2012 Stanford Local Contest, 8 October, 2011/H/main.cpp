#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

string diff(string A, string B) {
    string C(A.size(), ' ');
    for (int i = 0; i < int(A.size()); ++i)
        C[i] = (A[i] - B[i] + 26) % 26 + 'A';
    return C;
}

int main() {
    int N;

    while (cin >> N) {
        if (N == 0)
            break;

        vector<string> D(N);
        vector<string> G(N);
        int length = numeric_limits<int>::min();
        string THE;
        for (int i = 0; i < N; ++i) {
            string P, C; cin >> P >> C;
            D[i] = diff(C, P);
            G[i] = string(P.size(), '0');

            int k = 0;

            vector<int> pi(C.size());
            for (int i = 1; i < int(C.size()); ++i) {
                while (k > 0 and C[k] != C[i])
                    k = pi[k - 1];
                if (C[k] == C[i])
                    ++k;
                pi[i] = k;
            }

            k = 0;
            string DIFF = D[i];
            for (int i = 0; i < int(DIFF.size()); ++i) {
                while (k > 0 and C[k] != DIFF[i])
                    k = pi[k - 1];
                if (C[k] == DIFF[i])
                    ++k;
            }

            while (k > 0) {
                G[i][P.size() - k] = '1';
                k = pi[k - 1];
            }

            length = max(length, int(P.size()));
            if (int(P.size()) == length)
                THE = D[i];
        }

        bool ok = false;
        for (int i = 0; i < length; ++i) {
            ok = true;
            for (int j = 0; j < N; ++j)
                if (int(D[j].size()) > i && D[j][i] != THE[i]) {
                    ok = false;
                    break;
                }
            if (!ok)
                break;
            for (int j = 0; j < N; ++j)
                if (i + 1 < int(D[j].size()))
                    if (G[j][i + 1] == '0') {
                        ok = false;
                        break;
                    }
            if (ok) {
                cout << THE.substr(0, i + 1) << "\n";
                break;
            }
        }

        if (!ok)
            cout << "Impossible\n";
    }
}
