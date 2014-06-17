#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    int A[4];
    for (int i = 0; i < 4; ++i)
        cin >> A[i];

    int answer = A[0] * A[1] + A[2] * A[3];
    do {
        answer = max(answer, A[0] * A[1] + A[2] * A[3]);
    } while (next_permutation(A, A + 4));

    cout << answer << "\n";
}
