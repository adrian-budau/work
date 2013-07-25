#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    while (true) {
        int N; cin >> N;

        if (N == 0)
            break;

        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];

        sort(V.begin(), V.end());

        if (N % 2 == 1) {
            cout << V[V.size() / 2] << ".0\n";
        } else {
            int sum = V[V.size() / 2 - 1] + V[V.size() / 2];
            cout << sum / 2 << "." << (sum % 2) * 5 << "\n";
        }
    }
}
