#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>

using namespace std;

const int kMaxN = 30000005;

int gcd(int A, int B) {
    if (B == 0)
        return A;
    return gcd(B, A % B);
}

int main() {
    vector<int> answer(kMaxN);
    int now = 0;
    for (int d = 1; d < kMaxN; ++d) {
        for (int i = d; i < kMaxN; i += d) {
            int j = i ^ d;
            if (j < 1 || j > i || j % d)
                continue;
            ++now;
            //++answer[i];
        }
    }

    for (int i = 2; i < kMaxN; ++i)
        answer[i] += answer[i - 1] + now;

    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        cout << "Case " << test << ": ";

        int x; cin >> x;
        cout << answer[x] << "\n";
    }
}
