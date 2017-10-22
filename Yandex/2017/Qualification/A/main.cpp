#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    vector<bool> have(N + 1, false);
    int answer = 0;
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        if (!have[x - 1])
            ++answer;
        have[x] = true;
    }

    cout << answer << "\n";
}
