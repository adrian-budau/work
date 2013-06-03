#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <cassert>

using namespace std;

bitset<75> A[75];

int main() {
    int N;
    int test = 0;
    while (cin >> N) {
        ++test;

        for (int i = 0; i < N; ++i) {
            A[i].reset(); 
            string S; cin >> S;
            for (int j = 0; j < N; ++j)
                if (S[j] == '1')
                    A[i][j] = 1;
            A[i][i] = 1;
        }

        bitset<75> M;
        for (int i = 0; i < N; ++i)
            M[i] = 1;

        vector<int> answer[7];
        for (int i = 0; i < N; ++i) {
            if (A[i] == M and not answer[1].size())
                answer[1].push_back(i + 1);

            for (int j = i + 1; j < N ; ++j) {
                if ((A[i] | A[j]) == M and not answer[2].size()) {
                    answer[2].push_back(i + 1);
                    answer[2].push_back(j + 1);
                }

                for (int k = j + 1; k < N; ++k) {
                    if ((A[i] | A[j] | A[k]) == M and not answer[3].size()) {
                        answer[3].push_back(i + 1);
                        answer[3].push_back(j + 1);
                        answer[3].push_back(k + 1);
                    }

                    for (int p = k + 1; p < N; ++p) {
                        if ((A[i] | A[j] | A[k] | A[p]) == M and not answer[4].size()) {
                            answer[4].push_back(i + 1);
                            answer[4].push_back(j + 1);
                            answer[4].push_back(k + 1);
                            answer[4].push_back(p + 1);
                        }

                        for (int l = p + 1; l < N; ++l)
                            if ((A[i] | A[j] | A[k] | A[p] | A[l]) == M and not answer[5].size()) {
                                answer[5].push_back(i + 1);
                                answer[5].push_back(j + 1);
                                answer[5].push_back(k + 1);
                                answer[5].push_back(p + 1);
                                answer[5].push_back(l + 1);
                            }
                    }
                }
            }
        }

        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            V[i] = i;

        while (V.size() > 0) {
            int maxt = 0, where = 0;
            for (int i = 0; i < int(V.size()); ++i) {
                int aux = 0;
                for (int j = 0; j < int(V.size()); ++j)
                    if (A[V[i]][V[j]])
                        ++aux;
                if (aux > maxt)
                    maxt = aux, where = V[i];
            }

            vector<int> newV;
            answer[6].push_back(where + 1);
            for (int i = 0; i < int(V.size()); ++i)
                if (A[where][V[i]] == 0)
                    newV.push_back(V[i]);
            swap(V, newV);
        }

        bitset<75> X;
        for (int i = 0; i < int(answer[6].size()); ++i) {
            X |= A[answer[6][i] - 1];
        }
        assert(X == M);

        cout << "Case " << test << ": ";
        for (int i = 1; i <= 6; ++i)
            if (answer[i].size()) {
                cout << i;
                for (int j = 0; j < i and j < int(answer[i].size()); ++j)
                    cout << " " << answer[i][j];
                assert(i != 6 or answer[i].size() == 6u);
                break;
            }
        cout << "\n";
    }
}
