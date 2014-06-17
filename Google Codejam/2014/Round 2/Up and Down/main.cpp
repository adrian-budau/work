#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N; cin >> N;

        vector<int> A(N);
        for (int i = 0; i < N; ++i)
            cin >> A[i];

        int answer = 0;
        for (int i = 0; i < N; ++i) {
            int position = min_element(A.begin(), A.end()) - A.begin();
            answer += min<int>(position, A.size() - position - 1);
            A.erase(A.begin() + position);
        }

        cout << "Case #" << test << ": " << answer << "\n";
    }
}
