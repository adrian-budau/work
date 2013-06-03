#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector< vector<int> > mul(vector< vector<int> > A, vector<vector<int> > B, int P) {
    int N = A.size();
    int M = B.size();
    int O = B[0].size();

    vector< vector<int> > C(N, vector<int>(O, 0));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            for (int k = 0; k < O; ++k)
                C[i][k] = (C[i][k] + A[i][j] * B[j][k]) % P;
    return C;
}

void div(vector<int> &nr, int p) {
    int rest = 0;
    for (int i = nr[0]; i > 0; --i) {
        nr[i] += rest * 10;
        rest = nr[i] % p;
        nr[i] /= p;
    }

    while (nr[0] > 1 and nr[nr[0]] == 0)
        --nr[0];
}

int main() {
    ifstream cin("nice.in");
    ofstream cout("nice.out");

    string N; int M, P; cin >> N >> M >> P;

    vector< vector<int> > Z(1 << M, vector<int>(1 << M, 0));
    for (int i = 0; i < (1 << M); ++i) {
        vector<int> A(M);
        for (int k = 0; k < M; ++k)
            if ((1 << k) & i)
                A[k] = 1;
        for (int j = 0; j < (1 << M); ++j) {
            vector<int> B(M);
            for (int k = 0; k < M; ++k)
                if ((1 << k) & j)
                    B[k] = 1;
            bool ok = true;
            for (int k = 0; k < M - 1; ++k)
                if (A[k] == A[k + 1] and A[k] == B[k] and B[k] == B[k + 1])
                    ok = false;
            if (ok)
                Z[i][j] = 1;
        }
    }

    vector< vector<int> > base(1, vector<int>(1 << M, 0));
    base[0][10 & ((1 << M) - 1)] = 1;

    reverse(N.begin(), N.end());
    vector<int> nr(N.size() + 1);
    for (int i = 0; i < int(N.size()); ++i)
        nr[i + 1] = N[i] - '0';
    nr[0] = N.size();

    while (nr[0] != 1 or nr[1] != 0) {
        if (nr[1] % 2)
            base = mul(base, Z, P);
        div(nr, 2);
        Z = mul(Z, Z, P);
    }

    int answer = 0;
    for (int i = 0; i < (1 << M); ++i)
        answer += base[0][i];

    cout << answer % P << "\n";
}
