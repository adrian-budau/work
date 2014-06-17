#include <iostream>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N, K, C; cin >> N >> K >> C;

        int best = N;
        {
            int type = K / N;
            if (type * N >= C)
                best = 0;
        }

        for (int wrongs = 1; wrongs < N and best; ++wrongs) {
            int remainder = K % (N - wrongs), X = 0;
            if (remainder > K - C)
                X = remainder - (K - C);

            if (X / wrongs < (K - X) / (N - wrongs)) {
                best = wrongs;
                break;
            }
        }

        cout << "Case #" << test << ": " << best + C << "\n";
    }
}
