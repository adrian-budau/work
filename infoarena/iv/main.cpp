#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int kModulo = 3210121;

inline void add(int& a, int b) {
    (a += b) >= kModulo ? a -= kModulo : a;
}

int main() {
    ifstream cin("iv.in");
    ofstream cout("iv.out");

    string A, B; cin >> A >> B;

    vector< vector<int> > now(A.size() + 1, vector<int>(B.size() + 1, 0));
    now[0][0] = 1;

    int answer = 0;
    for (int i = 0; i <= int(A.size()); ++i) {
        vector< vector<int> > next(A.size() + 1, vector<int>(B.size() + 1, 0));

        for (int j = 0; j <= int(A.size()); ++j)
            for (int k = 0; k <= int(B.size()); ++k) {
                int l = i + k - j;
                if (i + j > int(A.size()))
                    continue;
                if (k + l > int(B.size()))
                    continue;

                if ((i + j + k + l) / 2 == int(A.size() + B.size()) / 2)
                    add(answer, now[j][k]);

                if (i < int(A.size()) and l < int(B.size()))
                    if (A[i] == B[B.size() - l - 1])
                        add(next[j][k], now[j][k]);
                if (i < int(A.size()) and j < int(A.size()))
                    if (A[i] == A[A.size() - j - 1])
                        add(next[j + 1][k], now[j][k]);
                if (j < int(A.size()) and k < int(B.size()))
                    if (A[A.size() - j - 1] == B[k])
                        add(now[j + 1][k + 1], now[j][k]);
                if (k < int(B.size()) and l < int(B.size()))
                    if (B[k] == B[B.size() - l - 1])
                        add(now[j][k + 1], now[j][k]);
            }
        swap(now, next);
    }

    cout << answer << "\n";
}
