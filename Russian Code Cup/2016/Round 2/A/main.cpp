#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int N, T, T0; cin >> N >> T >> T0;

    for (int i = 0; i < N; ++i) {
        int M; cin >> M;
        vector<int> V(M);
        for (int i = 0; i < M; ++i)
            cin >> V[i];
        int answer = 0;
        int aux = 0;
        multiset<int> S;
        for (answer = 0; answer < M && aux + V[answer] <= T; ++answer) {
            S.insert(V[answer]);
            aux += V[answer];
        }
        if (answer < M) {
            aux += V[answer];
            S.insert(V[answer]);
            aux -= *S.rbegin();
            aux += T0;
            if (aux <= T)
                for (++answer; answer < M && aux + V[answer] <= T; ++answer)
                    aux += V[answer];
        }
        cout << answer << "\n";
    }
}
