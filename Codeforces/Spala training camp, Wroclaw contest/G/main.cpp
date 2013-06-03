#include <iostream>
#include <vector>

using namespace std;

const int kMaxV = 200005;

int H[kMaxV * 2];

int main() {
    int N; cin >> N;

    int answer = 0;
    vector<int> V(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i];
        for (int j = 0; j < i; ++j)
            if (H[V[i] - V[j] + kMaxV]) {
                ++answer;
                break;
            }
        for (int j = 0; j <= i; ++j)
            H[V[i] + V[j] + kMaxV] = 1;
    }

    cout << answer << "\n";
}
