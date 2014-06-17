#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

using namespace std;

vector< pair< pair<int, int>, pair<int, int> > > A, B;
vector< pair<int, int> > C, D;

ifstream fin("bilete.in");
ofstream fout("bilete.out");

void back(int now, int K) {
    if (K == 0) {
        for (int i = 0; i < int(B.size()); ++i)
            tie(C[i], D[i]) = B[i];

        sort(C.begin(), C.end());
        sort(D.begin(), D.end());

        if (C <= D) {
            for (int i = 0; i < int(C.size()); ++i)
                fout << char(C[i].first + '0') << char(C[i].second + '0');
            fout << "\n";
        }
        return;
    }

    if (now == int(A.size()))
        return;

    B.push_back(A[now]);
    back(now + 1, K - 1);
    B.pop_back();
    back(now + 1, K);
}

int main() {
    int N, K; fin >> N >> K;

    for (int i = 1; i <= N; ++i)
        for (int j = 1; j <= N; ++j)
            A.push_back({{i, j}, {i, N - j + 1}});

    C.resize(K); D.resize(K);
    back(0, K);
}
