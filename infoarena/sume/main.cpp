#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("sume.in");
    ofstream cout("sume.out");

    int P; cin >> P;
    int N;
    for (N = 1; N * (N - 1) / 2 < P; ++N);
    if (N * (N - 1) / 2 != P) {
        cout << "-1\n";
        return 0;
    }

    vector<int> A(P);
    for (int i = 0; i < P; ++i)
        cin >> A[i];
    sort(A.begin(), A.end());

    for (int i = 2; i < N; ++i) {
        int S = A[0] + A[1] + A[i];
        if(S % 2)
            continue;
        S /= 2;
        vector<int> answer(N);
        answer[0] = S - A[i];
        answer[1] = S - A[1];
        answer[2] = S - A[0];

        unordered_map<int, int> MT;
        MT[A[0]]++;
        MT[A[1]]++;
        MT[A[i]]++;
        int pos = 3;
        for (int j = 0; j < P; ++j)
            if (MT[A[j]] > 0)
                --MT[A[j]];
            else {
                if (pos == N) {
                    pos = -1;
                    break;
                }

                answer[pos] = A[j] - answer[0];
                for (int k = 1; k < pos; ++k)
                    ++MT[answer[k] + answer[pos]];
                ++pos;
            }
        if (pos == N) {
            cout << N << "\n";
            for (auto &x : answer)
                cout << x << " ";
            cout << "\n";
            return 0;
        }
    }
    cout << "-1\n";
}

