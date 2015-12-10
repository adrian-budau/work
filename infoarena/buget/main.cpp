#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("buget.in");
    ofstream cout("buget.out");

    int N; int64_t B; cin >> N >> B;

    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    sort(V.begin(), V.end());

    int64_t past = 0;
    for (int i = 0; i < N; ++i) {
        int64_t aux = past + 1LL * V[i] * (N - i);
        if (aux > B) {
            int last = i > 0 ? V[i - 1] : 0;
            aux = 1LL * last * (N - i) + past;
            int64_t left = B - aux;
            int64_t C = left / (N - i) + last;
            cout << C << "\n";
            return 0;
        }

        past += V[i];
    }
    cout << V.back() << "\n";
}
