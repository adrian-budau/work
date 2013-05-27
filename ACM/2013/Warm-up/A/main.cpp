#include <iostream>
#include <fstream>

using namespace std;

long long pow(int x, int y) {
    long long answer = 1;
    for (int i = 0; i < y; ++i)
        answer *= x;
    return answer;
}

long long solve(int K, int A, int B) {
    if (K == 0)
        return 1;
    if (B <= (1 << (K - 1)))
        return 2 * solve(K - 1, A, B);
    if (A > (1 << (K - 1)))
        return solve(K - 1, A - (1 << (K - 1)), B - (1 << (K - 1)));
    if (A <= 1 and B >= (1 << K))
        return pow(3, K);

    return 2 * solve(K - 1, A, B) + solve(K - 1, A - (1 << (K - 1)), B - (1 << (K - 1)));
}

int main() {
    int T; cin >> T;
    for (int i = 1; i <= T; ++i) {
        int K, A, B; cin >> K >> A >> B;

        cout << "Case " << i << ": " << solve(K, A, B) << "\n";
    }
}
